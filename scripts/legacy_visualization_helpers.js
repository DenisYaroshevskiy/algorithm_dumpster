function transformMeasurementURLs(benchDescription, type) {
  benchDescription.measurements = benchDescription.measurements.map(
    (m) => {
      m.url = OLD_ALGORITHM_DUMPSTER_URL + m.url.replace('{}', type);
      return m;
    }
  );
  return benchDescription;
}

async function visualizeLegacyTimings(divId, template, types) {
  visualizeBecnhmarksForTypes(divId, template, types,
    (benchDescription, type) => {
      return transformMeasurementURLs(benchDescription, type);
    });
}

async function visualizeLegacyCounts(divId, jsonBenchmarkDescription) {
  let benchmarkDescription = await fetch(jsonBenchmarkDescription);
  benchmarkDescription = await loadBenchmarkDescription(await benchmarkDescription.json());
  benchmarkDescription.measurements = OLD_ALGORITHM_DUMPSTER_URL + benchmarkDescription.measurements;
  let algorithmSettings = await loadAlgorithmSettings(benchmarkDescription);
  let data = await fetch(benchmarkDescription.measurements);
  data = await data.json();

  visualizeCountingBenchmark(divId, benchmarkDescription, algorithmSettings, data);
}
