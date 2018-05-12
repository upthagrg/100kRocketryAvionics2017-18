#!/usr/bin/python
import sys, decimal
import radio_interface as radio

lat_data, lon_data, alt_data, spd_data = [], [], [], []
lat_coeffs, lon_coeffs, alt_coeffs, spd_coeffs = [], [], [], []

def drange(start, stop, step):
  while start <= stop:
    yield float(start)
    start += float(step)


def interp_coeffs(data):
	n = len(data)
	xvals = [x for x, y in data]
	yvals = [y for x, y in data]
	diff = []
	for i in xrange(n):
		diff.append([0 for j in xrange(n)])
		diff[-1][0] = yvals[i]

	for i in xrange(1, n):
		for j in xrange(i, n):
			diff[j][i] = (diff[j][i-1] - diff[j-1][i-1]) / (xvals[j] - xvals[j-i])

	return [diff[i][i] for i in xrange(n)]


def get_interp_points(start, stop, step, degree):
	global lat_coeffs, lon_coeffs, alt_coeffs, spd_coeffs
	xvals = list(drange(start, stop, step))

	if len(lat_coeffs) != len(lat_data):
		lat_coeffs = interp_coeffs(lat_data)
		lon_coeffs = interp_coeffs(lon_data)
		alt_coeffs = interp_coeffs(alt_data)
		spd_coeffs = interp_coeffs(spd_data)

	degree = min(degree, len(lat_data) - 1)
	lats, lons, alts, spds = [], [], [], []

	for i in xrange(len(xvals)):
		lats.append(lat_coeffs[degree])
		lons.append(lon_coeffs[degree])
		alts.append(alt_coeffs[degree])
		spds.append(spd_coeffs[degree])
			
		for j in xrange(1, degree+1):
			lats[i] = lat_coeffs[degree-j] + (xvals[i] - lat_data[degree-j][0]) * lats[i]
			lons[i] = lon_coeffs[degree-j] + (xvals[i] - lon_data[degree-j][0]) * lons[i]
			alts[i] = alt_coeffs[degree-j] + (xvals[i] - alt_data[degree-j][0]) * alts[i]
			spds[i] = spd_coeffs[degree-j] + (xvals[i] - spd_data[degree-j][0]) * spds[i]

	return zip(xvals, lats, lons, alts, spds)


def main():
	if len(sys.argv) < 5:
		print "Usage:", sys.argv[0], "<start> <stop> <step> <degree>"
		exit(0)
	
	radio.seed_random()

	while True:
		(time, lat, lon, alt, spd) = radio.extract_packet()
		lat_data.append((time, lat))
		lon_data.append((time, lon))
		alt_data.append((time, alt))
		spd_data.append((time, spd))
		print str.join(")\n", str(get_interp_points(float(sys.argv[1]), float(sys.argv[2]), decimal.Decimal(sys.argv[3]), int(sys.argv[4]))).strip("[]").split("),"))


if __name__ == "__main__":
	main()
