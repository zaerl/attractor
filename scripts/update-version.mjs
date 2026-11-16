/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

import { readFileSync, writeFileSync } from 'fs';

function getVersion() {
  const version = readFileSync('./VERSION', 'utf-8').trim().split('.');
  const major = parseInt(version[0]);
  const minor = parseInt(version[1]);
  const revision = parseInt(version[2]);

  return {
    major,
    minor,
    revision,
    version: `${major}.${minor}.${revision}`,
    versionShort: `${major}.${minor}`,
    number: major << 8 | minor << 4 | revision,
  }
}

function substituteBlock(fileContent, start, end, replacement) {
  const startIndex = fileContent.indexOf(start) + start.length;
  const endIndex = end === null ? null : fileContent.indexOf(end, startIndex);

  const ret = fileContent.slice(0, startIndex) + replacement;

  if(endIndex !== null) {
    return ret + fileContent.slice(endIndex);
  } else {
    return ret + "\n";
  }
}

let fileContent = readFileSync('./attractor.h', 'utf-8');
const v = getVersion();
const hexNumber = '0x' + v.number.toString(16).toUpperCase();

fileContent = substituteBlock(fileContent, '#define ATT_VERSION_NUMBER', '\n',
` ${hexNumber} // MAJOR << 8 | MINOR << 4 | REVISION`);
fileContent = substituteBlock(fileContent, '#define ATT_VERSION_MAJOR', '\n', ' ' + v.major.toString());
fileContent = substituteBlock(fileContent, '#define ATT_VERSION_MINOR', '\n', ' ' + v.minor.toString());
fileContent = substituteBlock(fileContent, '#define ATT_VERSION_REVISION', '\n', ' ' + v.revision.toString());

fileContent = substituteBlock(fileContent, '#define ATT_VERSION "', '"\n', v.version);

writeFileSync('./attractor.h', fileContent);

const steps = [
`Update CHANGELOG.md: add a [${v.version}] section`,
`git commit --signoff -am "Update version to ${v.version}"`,
`git tag v${v.version}`,
'git push && git push origin --tags'
];

console.log(`\nVersion updated to ${v.version}\n`);
steps.forEach((step, i) => console.log(`${i + 1}. ${step}`));
