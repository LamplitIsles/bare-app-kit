const test = require('brittle')

const calls = []
let select = null

const bindingPath = require.resolve('./binding')
require.cache[bindingPath] = {
  id: bindingPath,
  filename: bindingPath,
  loaded: true,
  exports: {
    statusItemInit(systemImageName, accessibilityDescription, receiver, callback) {
      select = callback.bind(receiver)
      calls.push(['init', systemImageName, accessibilityDescription])
      return {}
    },
    statusItemAddItem(_handle, id, title, enabled) {
      calls.push(['add', id, title, enabled])
    },
    statusItemAddSeparator() {
      calls.push(['separator'])
    },
    statusItemUpdateItem(_handle, id, title, hasTitle, enabled, hasEnabled) {
      calls.push(['update', id, title, hasTitle, enabled, hasEnabled])
    },
    statusItemDestroy() {
      calls.push(['destroy'])
    },
    windowInit() {
      return {}
    },
    windowHidesOnClose(_handle, value) {
      calls.push(['hidesOnClose', value])
      return value === undefined ? false : undefined
    },
    windowHide() {
      calls.push(['hide'])
    },
    windowShow() {
      calls.push(['show'])
    },
    windowClose() {
      calls.push(['close'])
    }
  }
}

const StatusItem = require('./lib/status-item')
const Window = require('./lib/window')

test('builds and updates a status menu', (t) => {
  const item = new StatusItem({
    systemImageName: 'network',
    accessibilityDescription: 'Kepos'
  })

  item.addItem('status', 'Starting', { enabled: false }).addSeparator()
  item.addItem('quit', 'Quit').updateItem('status', { title: 'Online' })

  t.alike(calls.slice(0, 5), [
    ['init', 'network', 'Kepos'],
    ['add', 'status', 'Starting', false],
    ['separator'],
    ['add', 'quit', 'Quit', true],
    ['update', 'status', 'Online', true, false, false]
  ])
})

test('uses the packaged application icon when no symbol is supplied', (t) => {
  new StatusItem({ accessibilityDescription: 'Kepos' })

  t.alike(calls.at(-1), ['init', '', 'Kepos'])
})

test('validates status menu before native mutation', async (t) => {
  const item = new StatusItem({ systemImageName: 'network' })
  item.addItem('status', 'Starting')
  const count = calls.length

  await t.exception.all(() => item.addItem('status', 'Again'), /unique/)
  await t.exception.all(() => item.updateItem('missing', { title: 'x' }), /Unknown/)
  await t.exception.all(() => item.addItem(1, 'x'), /string/)
  await t.exception.all(() => item.addItem('nul\0id', 'x'), /NUL/)
  await t.exception.all(() => item.addItem('nul-title', 'x\0y'), /NUL/)
  await t.exception.all(() => item.updateItem('status', { title: 'x\0y' }), /NUL/)
  t.is(calls.length, count)
})

test('rejects NUL in status item construction before native mutation', async (t) => {
  const count = calls.length
  await t.exception.all(() => new StatusItem({ systemImageName: 'net\0work' }), /NUL/)
  t.is(calls.length, count)
})

test('forwards selection and destroys once', async (t) => {
  const item = new StatusItem({ systemImageName: 'network' })
  let selected = null
  item.addItem('open', 'Open').on('select', (id) => {
    selected = id
  })
  select('open')
  t.is(selected, 'open')

  item.destroy()
  item.destroy()
  t.is(calls.filter((call) => call[0] === 'destroy').length, 1)
  await t.exception.all(() => item.addSeparator(), /destroyed/)
})

test('forwards close-to-hide window controls', (t) => {
  const window = new Window({ hidesOnClose: true })
  window.hide().show().close()

  t.ok(calls.some((call) => call[0] === 'hidesOnClose' && call[1] === true))
  t.ok(calls.some((call) => call[0] === 'hide'))
  t.ok(calls.some((call) => call[0] === 'show'))
})
