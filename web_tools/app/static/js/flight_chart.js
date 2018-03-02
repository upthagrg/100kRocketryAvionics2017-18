// flight chart

var chart_data = [];

//TODO api fetch launch time for now...

var lt = new Date('2018-02-28 07:00:00');

function initChartData() {
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
                $.each(data.result, function(index, value) {
                    lt.setSeconds(lt.getSeconds() + parseInt(value.time));
                    //console.log(parseInt(value.time));
                    //console.log(index +" : " + lt.getFullYear()+"-"+lt.getMonth()+"-"+lt.getDate()+" "+lt.getHours()+":"+lt.getMinutes()+":"+lt.getSeconds());
                });
            }
        }
    });
}
function formatData(data) {
    //console.log(data.result[0].altitude);
    // var modData = [];
    // data.results.forEach(function (d, i) {
    //     var item = ["param-" + d.param];
    //     d.val.forEach(function (j) {
    //         item.push(j);
    //     });
    //     modData.push(item);
    // });
}

function renderChart() {
    var chart = c3.generate({
        data: {
            x: 'date',
            xFormat: '%Y-%m-%d %H:%M:%S',
            columns: chart_data

        },
        axis: {
            x: {
                type: "timeseries"
            }
        },
        line: {
            connectNull: true
        }
    });
}

function updateFlightChart(data) {
    formatData(data)
    //renderChart()
}