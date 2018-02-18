const EXTRA_ANGLE = 15,
    whRatio = 2 / (Math.sin(deg2rad(EXTRA_ANGLE)) + 1.1),
    r = Math.min(window.innerWidth, window.innerHeight * whRatio) / 2,
    angleScale = d3.scaleLinear().domain([0, 100]).range([-90 - EXTRA_ANGLE, 90 + EXTRA_ANGLE])

// Size canvas
const svg = d3.select('#canvas1')
    .attr('width', r * 2)
    .attr('height', r * 2 * whRatio)
    .attr('viewBox', `${-r} ${-r} ${r * 2} ${r * 2 * whRatio}`)

const svg2 = d3.select('#canvas2')
    .attr('width', r * 2)
    .attr('height', r * 2 * whRatio)
    .attr('viewBox', `${-r} ${-r} ${r * 2} ${r * 2 * whRatio}`)

// Add axis
svg.append('g').classed('axis', true)
    .call(d3.axisRadialInner(
        angleScale.copy().range(angleScale.range().map(deg2rad)),
        r - 5
    ))
svg2.append('g').classed('axis', true)
    .call(d3.axisRadialInner(
        angleScale.copy().range(angleScale.range().map(deg2rad)),
        r - 5
    ))

// Add needle
const needle = svg.append('g')
    .attr('transform', `scale(${r * 0.85})`)
    .append('path').classed('needle', true)
    .attr('d', ['M0 -1', 'L0.03 0', 'A 0.03 0.03 0 0 1 -0.03 0', 'Z'].join(' '))
    .attr('transform', `rotate(${angleScale(0)})`)
const needle2 = svg2.append('g')
    .attr('transform', `scale(${r * 0.85})`)
    .append('path').classed('needle', true)
    .attr('d', ['M0 -1', 'L0.03 0', 'A 0.03 0.03 0 0 1 -0.03 0', 'Z'].join(' '))
    .attr('transform', `rotate(${angleScale(0)})`)

// Add val label
const label = svg.append('text').classed('label', true)
    .attr('x', 0)
    .attr('y', r * 0.2)
    .attr('text-anchor', 'middle')
    .text('0')
const label2 = svg2.append('text').classed('label', true)
    .attr('x', 0)
    .attr('y', r * 0.2)
    .attr('text-anchor', 'middle')
    .text('0')

function setPosition(value, value2) {
    const newVal = Math.round(value)
    const newVal2 = Math.round(value2)
    label.text(newVal)
    label2.text(newVal2)
    //label.text(value)
    needle.transition().duration(1).ease(d3.easePoly)
        .attr('transform', `rotate(${angleScale(value)})`)
    needle2.transition().duration(1).ease(d3.easePoly)
        .attr('transform', `rotate(${angleScale(value2)})`)
}


function deg2rad(deg) { return deg * Math.PI / 180 }
var i=0.0
setInterval(() => {
    if(i<100.0){
      setPosition(i+=0.1, i);
    }
}, 1.0)
//setInterval(() => {
//      setPosition((Math.random()*100)+1, (Math.random()*100)+1);
//}, 1000.0)

