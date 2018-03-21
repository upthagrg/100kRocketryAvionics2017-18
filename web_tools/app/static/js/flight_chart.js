// flight chart
function initChartData() {
    $.ajaxSetup({
        cache: false
    });
    (function poll() {
        setTimeout(function () {
            $.ajax({
                cache: false,
                dataType: "json",
                url: "http://0.0.0.0:5000/api/v1.0/telemetry_all?_=" + (new Date).getTime(),
                method: 'GET',
                success: function (data) {
                    if (data.result == "no data") {
                        console.log("No data")
                        return
                    } else {
                        initFlightChart(data)
                        console.log(data)
                        poll();
                    }
                }
            });
        }, 1000);
    })();
}




function formatData(data) {
    var formated_data = [['date'], ['Sustainer'], ['Booster']]
    $.each(data[1].sustainer, function (index, packet) {
        formated_data[0].push(packet.time)
        formated_data[1].push(packet.altitude)
    })
    $.each(data[0].booster, function (index, packet) {
        formated_data[2].push(packet.altitude)
    }) 
    return formated_data
}

function renderChart(fomarted_data) {
    var chart = c3.generate({
        data: {
            x: 'date',
            xFormat: '%Y-%m-%d %H:%M:%S',
            columns: fomarted_data

        },
        zoom: {
            enabled: true
        },
        transition: {
            duration: 100
        },
        axis: {
            x: {
                type: "timeseries",
                fit: true,
                tick: {
                    count: 10,
                    format: '%Y-%m-%d %H:%M:%S'
                },
                localtime: false,
            }
        },
        line: {
            connectNull: true
        }
    });
}

function initFlightChart(data) {
    fomarted_data = formatData(data)
    renderChart(fomarted_data)
}