const { execFileSync } = require('child_process')
const { mkdtempSync, rmSync } = require('fs')
const { tmpdir } = require('os')
const { join, resolve } = require('path')

const repository = resolve(__dirname, '..')
const root = resolve(repository, '../../..')
const bin = (name) => join(root, 'node_modules', '.bin', name)
const build = join(repository, 'test', 'addon', 'build')
const prebuilds = join(repository, 'test', 'addon', 'prebuilds')
const output = mkdtempSync(join(tmpdir(), 'bare-app-kit-lifecycle-'))

try {
  run('bare-make', [
    'generate',
    '--source',
    join(repository, 'test', 'addon'),
    '--build',
    build,
    '--platform',
    'darwin',
    '--arch',
    'arm64',
    '--define',
    `CMAKE_PREFIX_PATH:PATH=${join(root, 'node_modules')}`
  ])
  run('bare-make', ['build', '--build', build, '--clean'])
  run('bare-make', ['install', '--build', build, '--prefix', prebuilds])
  run('bare-build', [
    '--base',
    repository,
    '--host',
    'darwin-arm64',
    '--out',
    output,
    '--runtime',
    'bare-app-kit/runtime',
    '--identifier',
    'io.github.ttalab.bare-app-kit-testing',
    '--name',
    'LifecycleTest',
    join(repository, 'test', 'status-item-refs.js')
  ])
  run('bare-link', [
    '--host',
    'darwin-arm64',
    '--out',
    join(output, 'LifecycleTest.app', 'Contents', 'Frameworks'),
    join(repository, 'test', 'addon')
  ])
  execFileSync(join(output, 'LifecycleTest.app', 'Contents', 'MacOS', 'LifecycleTest'), {
    stdio: 'inherit'
  })
} finally {
  rmSync(output, { force: true, recursive: true })
}

function run(name, args) {
  execFileSync(bin(name), args, { cwd: repository, stdio: 'inherit' })
}
