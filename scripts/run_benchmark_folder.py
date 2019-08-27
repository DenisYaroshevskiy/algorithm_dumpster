"""
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
 *
"""

import argparse
import glob
import json
import ntpath
import subprocess
import os


def parseOptions():
    parser = argparse.ArgumentParser(
        description="Runs all benchmarks from a folder. Output: folder of jsons, that can be displayed")
    parser.add_argument('template', metavar='template')
    parser.add_argument('benchmarks_folder', metavar='benchmarks_folder')
    parser.add_argument('where_to_create_output', metavar='out')
    options = parser.parse_args()

    return [options.template, options.benchmarks_folder, options.where_to_create_output]


def runBench(bench, out):
    args = [bench, '--benchmark_out_format=json', '--benchmark_out=' + out]
    subprocess.call(args)


def generateDescription(benchName, template, names, outs):
    res = {}
    res['base'] = template
    res['general'] = {}
    res['general']['title'] = benchName.replace('_', ' ')
    res['measurements'] = [{ "name" : name, "url": url } for name, url in zip(names, outs)]

    return res

def writeDescription(benchName, whereTo, template, names, outs):
  description = generateDescription(benchName, template, names, outs)
  with open(whereTo + '/description.json', 'w') as outfile:
    json.dump(description, outfile, sort_keys=True, indent=2)


if __name__ == '__main__':
    [template, benchmarkFolder, whereTo] = parseOptions()

    benchName = ntpath.basename(benchmarkFolder)
    whereTo = whereTo + '/' + benchName
    if not os.path.exists(whereTo):
        os.makedirs(whereTo)

    benchmarks = [b for b in glob.glob(benchmarkFolder + "/*")]
    names = [ntpath.basename(b) for b in benchmarks]
    benchmarksOut = [whereTo + '/' + n + '.json' for n in names]

    for bench, out in zip(benchmarks, benchmarksOut):
        runBench(bench, out)

    writeDescription(benchName, whereTo, template, names, benchmarksOut)
