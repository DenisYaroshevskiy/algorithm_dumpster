"""
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
 *
"""

import argparse
import io
import os
import re

userIncludesParser = re.compile( r'\s*#\s*include\s*"(.*)"' )


def parseOptions():
    parser = argparse.ArgumentParser(
        description='A very crude generator of a single header.'
                    'Given a header, finds dependencies and collapses everything in one.'
                    'Mostly to use with godbolt')
    parser.add_argument('header', metavar='header')
    parser.add_argument('generatedHeader', metavar='generatedHeader')
    options = parser.parse_args()

    return [options.header, options.generatedHeader]



class ProcessedFile:
    def __init__(self, name, text, dependencies):
        self.name = name
        self.text = text
        self.dependencies = dependencies

def readFile(fileName):
    f = io.open(fileName, 'r', encoding='utf-8')
    return f

def updateFileName(rootFile, nextHeader):
    commonRoot = nextHeader.split('/')[0]
    idx = rootFile.find(commonRoot + '/')
    return rootFile[0:idx] + nextHeader

def parseFile(fileName):
    fileContents = readFile(fileName)
    headers = []
    text = ''
    for line in fileContents:
        m = userIncludesParser.match(line)
        if m:
            headers.append(updateFileName(fileName, m.group(1)))
        else:
            text += line
    return ProcessedFile(fileName, text, headers)


def main(fileName):
    toProcess = {}
    toVisit = [fileName]
    while len(toVisit) is not 0:
        top = toVisit.pop()
        if top in toProcess:
            continue

        parsed = parseFile(top)
        toProcess[top] = parsed
        toVisit.extend(parsed.dependencies)

    res = ''

    # We can do this so much smarter ...
    while len(toProcess) is not 0:
        nextToProcess = {}
        for fileName, parsed in toProcess.items():
            parsed.dependencies = [h for h in parsed.dependencies if h in toProcess]
            if len(parsed.dependencies) != 0:
                nextToProcess[fileName] = parsed
            else:
                print(fileName)
                res += parsed.text
        toProcess = nextToProcess

    return res


if __name__ == '__main__':
   header, generatedHeader = parseOptions()
   res = main(header)
   with open(generatedHeader, 'w') as f:
       f.write(res)
