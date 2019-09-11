/*
 * Copyright 2019 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

'use strict';

const predefinedStyles = [
    ["rgb(000, 0, 255)", "solid"],
    ["rgb(128, 0, 128)", "dot"],
    ["rgb(255, 0, 0)", "dash"],
    ["rgb(50, 50, 0)", "dashdot"],
    ["rgb(55, 128, 191)", "solid"],
];

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

function benchNameSplit(benchmarkDescription, name) {
    let parts = name.split('/');
    let x = Number(parts[2]);

    let percentage = undefined;
    if (benchmarkDescription.general.percentage_position !== undefined) {
        percentage = Number(parts[benchmarkDescription.general.percentage_position + 1]);
        x = percentage;
    }

    let size = undefined;
    if (benchmarkDescription.general.size_position !== undefined) {
        size = Number(parts[benchmarkDescription.general.size_position + 1]);
        if (benchmarkDescription.general.slice_percentage) {
            size = size * benchmarkDescription.general.slice_percentage / 100;
        }
    }

    let initial_size = undefined;
    if (benchmarkDescription.general.initial_size_position !== undefined) {
        initial_size = Number(parts[benchmarkDescription.general.initial_size_position + 1]);
        x = x * initial_size;
    }

    if (benchmarkDescription.general.convert_size_times_percentage_to_x) {
        console.assert(size !== undefined);
        console.assert(percentage !== undefined);

        x = size * percentage / 100;
    }

    return {
        str: parts[0],
        x
    };
}

function transformGoogleBenchmarkData(benchmarkDescription, loadedJson) {
    let result = {
        x: [],
        y: []
    };

    loadedJson.benchmarks.forEach((measurement) => {
        let x = benchNameSplit(benchmarkDescription, measurement.name).x;
        result.x.push(x);
        let y = measurement.real_time;
        result.y.push(measurement.real_time);
    });

    return result;
}

function tranformDataWithSettings(benchmarkDescription, loadedData) {
    if (benchmarkDescription.general.divide_y_by_x) {
        loadedData = loadedData.map((loaded) => {
          loaded.y = zipArrays(loaded.x, loaded.y).map((x_y) => x_y[1] / x_y[0]);
          return loaded;
        });
    }
    return loadedData;
}

function drawWithPlotly(element, benchmarkDescription, loadedData) {
    console.log('drawWithPlotly, benchmarkDescription:');
    console.log(benchmarkDescription);
    console.log('drawWithPlotly, loadedData:');
    console.log(loadedData);

    loadedData = tranformDataWithSettings(benchmarkDescription, loadedData);

    let traces = loadedData.map((loaded) => {
        return {
            name: loaded.name,
            x: loaded.x,
            y: loaded.y,
            marker: { size: 8 },
            line: {
                color: loaded.color,
                dash: loaded.dash,
                size: 3
            },
            type: 'scatter'
        };
    });


    let layout = {
        title: benchmarkDescription.general.title,
        width: 800,
        height: 600,
        yaxis: {
            rangemode: 'tozero',
            autorange: true
        }
    };

    if (benchmarkDescription.general.x_log) {
        if (!layout.xaxis) layout.xaxis = {};
        layout.xaxis.type = 'log';
    }

    Plotly.newPlot(element, traces, layout);
}

async function loadMeasurements(beenchmarkDescription, algorithmSettings) {
    let rawDataPromises = beenchmarkDescription.measurements.map(async (m) => {
        return fetch(m.url).then(
            async (raw) => {
                raw = await raw.json();
                const xs_and_ys = transformGoogleBenchmarkData(beenchmarkDescription, raw);
                const curSettings = algorithmSettings[m.name];
                console.log(curSettings);
                return {
                    name: curSettings.display_name,
                    x: xs_and_ys.x,
                    y: xs_and_ys.y,
                    color: curSettings.color,
                    dash: curSettings.dash,
                };
            }
        )
    });

    let rawData = await Promise.all(rawDataPromises);

    // TODO: transform the data: baseline, scaling by element.

    return rawData;
}

async function visualizeBecnhmark(element, benchmarkDescription, algorithmSettings) {
    let loaded = await loadMeasurements(benchmarkDescription, algorithmSettings);
    drawWithPlotly(element, benchmarkDescription, loaded);
}

function overrideWithDerived(base, derived) {
    Object.keys(derived).forEach((key) => {
        if (base[key]) {
            if (typeof (base[key]) !== 'string' && typeof (base[key]) !== 'number') {
                overrideWithDerived(base[key], derived[key]);
                return;
            }
        }
        base[key] = derived[key];
    });
    return base;
}

function populateStyles(algorithmSettingsSection) {
    let algorithms = Object.keys(algorithmSettingsSection);
    if (algorithms.length > predefinedStyles.length) {
        throw new Error('not enough line styles for section: ' + algorithms.toString());
    }
    zipArrays(algorithms, predefinedStyles).forEach((zippedObj) => {
        algorithmSettingsSection[zippedObj[0]].color = zippedObj[1][0];
        algorithmSettingsSection[zippedObj[0]].dash = zippedObj[1][1];
    });
}

async function loadBenchmarkDescription(description) {
    if (description['base']) {
        const loaded = await fetch(description.base);
        let base = await loadBenchmarkDescription(await loaded.json());
        return overrideWithDerived(base, description);
    }
    return description;
}

async function loadAlgorithmSettings(benchmarkDescription) {
    let allAlgorithmSettings = await fetch(benchmarkDescription.general.algorithm_settings_url);
    allAlgorithmSettings = await allAlgorithmSettings.json();
    let algorithmSettings = allAlgorithmSettings[benchmarkDescription.general.algorithm_settings_section];
    populateStyles(algorithmSettings);
    return algorithmSettings;
}

function makeBenchmarkWithSelection(div, options, drawSelected) {
    let select = document.createElement("select");
    let newDiv = document.createElement("div");

    options.forEach((o) => {
        select.options.add(new Option(o));
    });

    let frag = document.createDocumentFragment();
    frag.appendChild(select);
    frag.appendChild(newDiv);
    div.appendChild(frag);

    select.onchange = () => { drawSelected(newDiv, select.value); };
    drawSelected(newDiv, select.value);
}

async function visualizeBecnhmarksForTypes(elementID, template, types) {
    let element = document.getElementById(elementID);
    makeBenchmarkWithSelection(element, types, async (div, type) => {
        let jsonURL = template.replace('{}', type);
        let benchmarkDescription = await fetch(jsonURL);
        benchmarkDescription = await loadBenchmarkDescription(await benchmarkDescription.json());
        let algorithmSettings = await loadAlgorithmSettings(benchmarkDescription);
        visualizeBecnhmark(div, benchmarkDescription, algorithmSettings);
    });
}

function transformCountingData(benchmarkDescription, algorithmSettings, data, selected) {
    let namesToData = {};
    let hasNonZeroes = false;

    Object.keys(data).forEach((name) => {
        let {str, x} = benchNameSplit(benchmarkDescription, name);
        let y = data[name][selected];
        if (namesToData[str] === undefined) {
            namesToData[str] = {
                name: algorithmSettings[str].display_name,
                x: [],
                y: []
            };
        }
        namesToData[str].x.push(x);
        namesToData[str].y.push(y);
        if (y > 0) hasNonZeroes = true;
    });

    let res = [];
    Object.keys(namesToData).forEach((key) => res.push(namesToData[key]));

    if (hasNonZeroes) return res;
    return undefined;
}

function loadAllCountedKeys(benchmarkDescription, algorithmSettings, data) {
    let firstKey = Object.keys(data)[0];
    let firstValue = data[firstKey];
    let res = {};
    Object.keys(firstValue).forEach((key) => {
        let selected = transformCountingData(benchmarkDescription, algorithmSettings, data, key);
        if (!selected) return undefined;
        res[key] = selected;
    });
    return res;
}

async function visualizeCountingBenchmark(
    elementID, benchmarkDescription,
    algorithmSettings, data) {
    let element = document.getElementById(elementID);
    let allLoaded = loadAllCountedKeys(benchmarkDescription, algorithmSettings, data);

    makeBenchmarkWithSelection(element, Object.keys(allLoaded), (div, selected) => {
      drawWithPlotly(div, benchmarkDescription, allLoaded[selected]);
    });
}

async function visualizeCountingBenchmarkFromJson(elementID, jsonBenchmarkDescription) {
    let benchmarkDescription = await fetch(jsonBenchmarkDescription);
    benchmarkDescription = await loadBenchmarkDescription(await benchmarkDescription.json());
    let algorithmSettings = await loadAlgorithmSettings(benchmarkDescription);
    let data = await fetch(benchmarkDescription.measurements);
    data = await data.json();
    visualizeCountingBenchmark(elementID, benchmarkDescription, algorithmSettings, data);
}
