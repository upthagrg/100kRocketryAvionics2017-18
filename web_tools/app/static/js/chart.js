var data = [];
for (i = 0; i < 1000; i++) {
  data.push({ time: (Math.floor(Date.now() / 1000)+1000), y: (Math.random() * (500 - 40) + 40)})
}


var lineChartData = [
  // Altitude
  {
    label: "Altitude",
    values: data,
    range: [40,500]
  }
];