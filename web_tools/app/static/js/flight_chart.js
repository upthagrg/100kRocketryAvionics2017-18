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
                url: "http://0.0.0.0:5000/api/v1.0/telemetry_all/timestamp?_=" + (new Date).getTime(),
                method: 'GET',
                success: function (data) {
                    if (data.result == "no data") {
                        console.log("No data")
                        return
                    } else {
                        initFlightChart(data)
                        poll();
                    }
                }
            });
        }, 1000);
    })();
}

function formatData(data) {
    var formated_data = [['date'], ['Booster'], ['Sustainer']]
    $.each(data.result, function (index, value) {
        formated_data[0].push(value.time)
        if(value.type=="b") {
        formated_data[1].push(value.altitude)
        }
        else {
        formated_data[2].push(value.altitude)
        }
    });
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