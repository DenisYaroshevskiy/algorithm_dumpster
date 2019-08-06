'use strict';

function getName(benchmarLongkName) {
    return benchmarLongkName.split('<')[1].split('>')[0];
}

function transformGoogleBenchmarkOutput(loadedJson) {
    let result = {
        name: '',
        xs: [],
        times: []
    };
    result.name = getName(loadedJson.benchmarks[0].name);

    loadedJson.benchmarks.forEach(measurement => {
        let parts = measurement.name.split('/');
        result.xs.push(parts[parts.length - 1]);
        result.times.push(measurement.real_time);
    });

    return result;
}

function loadGoogleBenchmarkResults(file) {
    file = 'computed_jsons/' + file + '.json';
    return fetch(file).then(
        (resp) => {
            return resp.json();
        }
    ).then(
        (loadedJson) => {
            return transformGoogleBenchmarkOutput(loadedJson);
        }
    );
}

function zipArrays(arr1, arr2) {
    return arr1.map((x, i) => [x, arr2[i]]);
}

function subtractBaseline(main, baseline) {
    const baselineMap = new Map(zipArrays(baseline.xs, baseline.times));
    return {
        name: main.name,
        xs: main.xs,
        times: main.times.map((time, i) => {
            return time - baselineMap.get(main.xs[i]);
        })
    };
}

function loadBenchmark(benchmark) {
    const split = benchmark.split(':');
    const main = loadGoogleBenchmarkResults(split[0]);
    if (split.length === 1) {
        return main;
    }
    const baseline = loadGoogleBenchmarkResults(split[1]);
    return Promise.all([main, baseline]).then((ps) => subtractBaseline(ps[0], ps[1]));
}

function divideByX(bench) {
    bench.times = bench.times.map((time, i) => time / bench.xs[i])
    return bench;
}

function visualizeBenchmarks(id, element, settings, benchmarks) {
    google.charts.load('current', { packages: ['corechart'] });
    google.charts.setOnLoadCallback(drawChart);

    let loaded = benchmarks.map(loadBenchmark);
    if (settings.shouldDivideByX) {
        loaded = loaded.map((b) => b.then(divideByX));
    }

    function drawChart() {
        Promise.all(loaded).then(
            (benchmarks) => {
                const data = transformBenchmarksToDataTable(benchmarks);
                drawBenchmarksChart(id, element, data);
            }
        );
    }
}

function visualizeBenchmarksPloty(id, element, settings, benchmarks) {
    let loaded = benchmarks.map(loadBenchmark);
    if (settings.shouldDivideByX) {
        loaded = loaded.map((b) => b.then(divideByX));
    }

    let traces = Promise.all(loaded).then(
        (benchmarks) => {
            return benchmarks.map((b) => {
                return {
                    name: b.name,
                    x: b.xs,
                    y: b.times,
                    type: 'scatter'
                }
            });
        }
    )

    let layout = {
        title: id,
        width: 800,
        height: 600
    };

    if (settings.useLog) {
        layout.xaxis = { type : 'log'};
    }

    traces.then(
        (traces) => {
            Plotly.newPlot(element, traces, layout);
        }
    )
}

function visualizeAllSetsBenchmarks(id, element, settings, hasBaseline = false) {
    const names = ['absl', 'boost', 'srt', 'std', 'std_unordered'].map(
        (container) => {
            let res = id + '_' + container;
            if (hasBaseline) { res += ':' + id + '_baseline_' + container; }
            return res;
        }
    );
    visualizeBenchmarksPloty(id, element, settings, names);
}

function visualizeBenchmarksNoStd(id, element, settings, hasBaseline = false) {
    const names = ['absl', 'boost', 'srt'].map(
        (container) => {
            let res = id + '_' + container;
            if (hasBaseline) { res += ':' + id + '_baseline_' + container; }
            return res;
        }
    );
    visualizeBenchmarksPloty(id + '_no_std', element, settings, names);
}

function addNewChartDropDown(section) {
    let dropDown = document.createElement("section");
    let dropDownChart = document.createElement("div");
    dropDownChart.className = "benchmarkChart";
    dropDown.appendChild(dropDownChart);
    section.appendChild(dropDown);
    return dropDownChart;
}

function addBencharkCharts(id, settings) {
    let section = document.getElementById(id);
    visualizeAllSetsBenchmarks(id, addNewChartDropDown(section), settings);
    visualizeBenchmarksNoStd(id, addNewChartDropDown(section), settings);
}

function addBencharkChartsOnlyNoStd(id, settings) {
    let section = document.getElementById(id);
    visualizeBenchmarksNoStd(id, addNewChartDropDown(section), settings);
}

function addBencharkChartsWithBaseline(id, settings) {
    let section = document.getElementById(id);
    visualizeAllSetsBenchmarks(id, addNewChartDropDown(section), settings);
    visualizeBenchmarksNoStd(id, addNewChartDropDown(section), settings);
}
