/*
 * Copyright 2020 Denis Yaroshevskiy
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

function parseOneParameter(parameter) {
  const split = parameter.split(':');
  const key = split[0];
  let value = split.slice(1).join(':');
  if (!isNaN(value)) {
    value = parseInt((value));
  }
  return [key, value];
}

function parseMeasurement(measurement) {
  const parsed_parameters = measurement.name.split('/').slice(1).map(parseOneParameter);

  let res = Object.fromEntries(parsed_parameters);
  res.time = parseFloat(measurement.real_time);
  return res;
}

function isSubset(small, big) {
  for (const [key, value] of Object.entries(small)) {
    if (big[key] !== value) return false;
  }
  return true;
}

function setDifference(big, small) {
  let res = {}
  for (const [key, value] of Object.entries(big)) {
    if (!small.hasOwnProperty(key)) {
      res[key] = value;
    }
  }
  return res;
}

function arraysEqual(x, y) {
  if (x.length != y.length) return false;

  for (let i = 0; i < x.length; ++i) {
    if (x[i] !== y[i]) return false;
  }

  return true;
}

function partitionByKeys(objects, keys) {
  let res = [];
  for (const o of objects) {
    let indx = res.findIndex(e => {
      for (const key of keys) if (e[0][key] !== o[key]) return false;
      return true;
    });
    if (indx === -1) {
      indx = res.length;
      res.push([]);
    }
    res[indx].push(o);
  }

  return res;
}

const REDUCE_OPERATIONS = ['min', 'max', 'minmax'];

function processVaryingKeys(varying) {
  let x = undefined;
  let y = undefined;
  let selection = [];
  let reduce = undefined;
  for (const [key, value] of Object.entries(varying)) {
    if (value == 'x') {
      if (x) throw new Error(`duplicated x: ${x}, ${key}`);
      x = key;
      continue;
    }
    if (value == 'y') {
      if (y) throw new Error(`duplicated y: ${y}, ${key}`);
      y = key;
      continue;
    }
    if (value == 'selection') {
      selection.push(key);
      continue;
    }
    if (REDUCE_OPERATIONS.indexOf(value) !== -1) {
      if (reduce) throw new Error(`more then one reduce operation! Last one: ${key}`);
      reduce = value;
      continue;
    }
    throw new Error('unknown variation on ${key}: ${value}');
  }

  if (!x) throw new Error('x is not defined');
  if (!y) throw new Error('y is not defined');
  if (selection.length == 0) throw new Error('selection options are not specified');
  return { x, y, selection, reduce };
}

function minByKey(xs, key) {
  return xs.reduce((x1, x2) => x2[key] < x1[key] ? x2 : x1);
}

function maxByKey(xs, key) {
  return xs.reduce((x1, x2) => x2[key] >= x1[key] ? x2 : x1);
}

function reduceInGroups(objects, groupKey, redaction) {
  const sameKey = partitionByKeys(objects, [groupKey]);
  const valuesForKey = sameKey.map(redaction);
  return valuesForKey.flat();
}

function applyReduction(selections, varying) {
  if (!varying.reduce) return selections;

  if (varying.reduce === 'minmax') {
    varying.selection.push('variation');
  }

  let res = [];

  for (const sel of selections) {
    if (varying.reduce === 'min') {
      res.push(reduceInGroups(sel, varying.x, ms => minByKey(ms, varying.y)));
    } else if (varying.reduce === 'max') {
      res.push(reduceInGroups(sel, varying.x, ms => maxByKey(ms, varying.y)));
    } else if (varying.reduce === 'minmax') {
      let min = reduceInGroups(sel, varying.x, ms => minByKey(ms, varying.y));
      let max = reduceInGroups(sel, varying.x, ms => maxByKey(ms, varying.y));
      min = JSON.parse(JSON.stringify(min));
      max = JSON.parse(JSON.stringify(max));
      min.variation = 'min';
      max.variation = 'max';
      res.push(min);
      res.push(max);
    }
  }

  return res;
}

function makeSelectionObject(ms, varying) {
  const name = varying.selection.map(key => ms[0][key]).join('/');
  const x = ms.map(m => m[varying.x]);
  const y = ms.map(m => m[varying.y]);
  return { name, x, y };
}

function visualizationDataFromMeasurements(varying, fixed, measurements) {
  measurements = measurements.filter(e => isSubset(fixed, e));
  measurements = measurements.map(e => setDifference(e, fixed));
  varying = processVaryingKeys(varying);
  let selections = partitionByKeys(measurements, varying.selection);
  selections = applyReduction(selections, varying);
  return selections.map(ms => makeSelectionObject(ms, varying));
}

function drawBenchmark(element, data) {
  const traces = data.map(line => {
    return {
      name: line.name,
      x: line.x,
      y: line.y,
      marker: { size: 8 },
      line: { size: 3 },
      type: 'scatter'
    };
  });

  const layout = {
    width: 800,
    height: 600,
    yaxis: {
      rangemode: 'tozero',
      autorange: true
    }
  };

  Plotly.newPlot(element, traces, layout);
}

async function entryPoint(elementID) {
  const element = document.getElementById(elementID);
  const loaded = await fetch("data/bench/remove.json").then(async (raw) => raw.json());
  const measurements = loaded.benchmarks.map(parseMeasurement);
  const asVisualized = visualizationDataFromMeasurements(
    {
      percentage: 'x',
      time: 'y',
      padding: 'minmax',
      algorithm: 'selection',
    },
    {

      name: "remove 0s",
      type: "char",
      size: 50
    },
    measurements
  );
  drawBenchmark(element, asVisualized);
}

/// tests ---------------------------------

function exceptTrue(test, msg) {
  if (test) return;
  throw new Error(`Expectation failed: ${msg}`);
}

function expectEqual(expected, actual, msg = '') {
  const expectedStr = JSON.stringify(expected);
  const actualStr = JSON.stringify(actual);

  if (expectedStr !== actualStr) {
    throw new Error(`Expectation failed: expected=${expectedStr},  actual=${actualStr}\n${msg}`);
  }
}

function parseOneParameterTests() {
  expectEqual(["name", "remove 0s"], parseOneParameter("name:remove 0s"));
  expectEqual(["algorithm", "std::remove"], parseOneParameter("algorithm:std::remove"));
}

function parseMeasurementTests() {
  const measurement = {
    "name": "/name:remove 0s/size:50/type:char/algorithm:std::remove/percentage:5/padding:3",
    "run_name": "/name:remove 0s/size:50/type:char/algorithm:std::remove/percentage:5/padding:3",
    "run_type": "iteration",
    "repetitions": 0,
    "repetition_index": 0,
    "threads": 1,
    "iterations": 33770009,
    "real_time": 2.0909094161059294e+01,
    "cpu_time": 2.0909666384749833e+01,
    "time_unit": "ns"
  };

  const expected = {
    name: "remove 0s",
    size: 50,
    type: "char",
    algorithm: "std::remove",
    percentage: 5,
    padding: 3,
    time: measurement.real_time
  };

  expectEqual(expected, parseMeasurement(measurement));
}

function isSubsetTests() {
  const obj1 = { a: 'a', b: 3 };
  const obj2 = { a: 'a', b: 3, c: 'c' };
  exceptTrue(isSubset(obj1, obj2), "subset obj1, obj2");
  exceptTrue(!isSubset(obj2, obj1), "!subset obj2, obj1");

  const obj3 = { a: 'a', b: 3, d: 'd' };
  exceptTrue(!isSubset(obj3, obj2), "!subset obj2, obj1");
}

function setDifferenceTests() {
  const obj1 = { a: 'a', b: 3 };
  const obj2 = { a: 'a', b: 3, c: 'c' };
  const obj3 = setDifference(obj2, obj1);
  expectEqual({ c: 'c' }, obj3);
}

function arraysEqualTests() {
  const arr1 = [1, "123", 4];
  const arr2 = [1, "123", 5];

  exceptTrue(arraysEqual(arr1, arr1), "arr1 == arr1");
  exceptTrue(!arraysEqual(arr1, arr2), "arr1 != arr2");
}

function partitionByKeysTests() {
  const data = [
    {
      a: 1,
      b: 2,
      c: 3
    },
    {
      a: 1,
      b: 2,
      c: 4
    },
    {
      a: 2,
      b: 2,
      c: 4
    }
  ];

  {
    const keys = ['a', 'b'];
    expectEqual([[data[0], data[1]], [data[2]]], partitionByKeys(data, keys));
  }

  {
    const keys = ['b'];
    expectEqual([data], partitionByKeys(data, keys));
  }
}

function reductionsTests() {
  const data = [
    { a: 1 },
    { a: 0 }
  ];
  expectEqual(data[1], minByKey(data, 'a'));
  expectEqual(data[0], maxByKey(data, 'a'));
}

function visualizationDataFromMeasurementsTests() {
  const data = [
    {
      name: 'bench1',
      size: 1000,
      padding: 0,
      alg: 'alg1',
      variation: 0,
      percentage: 5,
      time: 0.1
    },
    {
      name: 'bench1',
      size: 1000,
      padding: 1,
      alg: 'alg1',
      variation: 0,
      percentage: 5,
      time: 0.25
    },
    {
      name: 'bench2',
      size: 1000,
      padding: 0,
      alg: 'alg1',
      variation: 0,
      percentage: 5,
      time: 0.25
    },
    {
      name: 'bench2',
      size: 1000,
      padding: 1,
      alg: 'alg1',
      variation: 0,
      percentage: 5,
      time: 0.25
    },
    {
      name: 'bench1',
      size: 1000,
      padding: 0,
      alg: 'alg2',
      variation: 1,
      percentage: 5,
      time: 0.4
    },
    {
      name: 'bench1',
      size: 1000,
      padding: 1,
      alg: 'alg2',
      variation: 1,
      percentage: 5,
      time: 0.3
    },
  ];

  const varying = {
    percentage: 'x',
    time: 'y',
    alg: 'selection',
    variation: 'selection',
    padding: 'min'
  };
  const fixed = {
    name: 'bench1',
    size: 1000
  };

  const expected = [
    {
      name: 'alg1/0',
      x: [5],
      y: [0.1]
    },
    {
      name: 'alg2/1',
      x: [5],
      y: [0.3]
    }
  ];

  expectEqual(expected, visualizationDataFromMeasurements(varying, fixed, data));
}

async function listOfTests() {
  parseOneParameterTests();
  parseMeasurementTests();
  isSubsetTests();
  setDifferenceTests();
  arraysEqualTests();
  partitionByKeysTests();
  reductionsTests();
  visualizationDataFromMeasurementsTests();
}

async function runTests(elementID) {
  let element = document.getElementById(elementID);

  try {
    listOfTests();
    element.innerHTML = `Tests passed`
  } catch (er) {
    element.innerHTML = `Tests failed. error=${er.message}`
  }
}
