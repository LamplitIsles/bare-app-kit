const EventEmitter = require('bare-events')
const binding = require('../binding')

module.exports = class AppKitStatusItem extends EventEmitter {
  constructor(opts = {}) {
    super()

    const { systemImageName = '', accessibilityDescription = '' } = opts
    assertNativeString(systemImageName, 'systemImageName')
    assertNativeString(accessibilityDescription, 'accessibilityDescription')

    this._destroyed = false
    this._ids = new Set()
    this._handle = binding.statusItemInit(
      systemImageName,
      accessibilityDescription,
      this,
      this._onselect
    )
  }

  addItem(id, title, opts = {}) {
    this._assertLive()
    assertNativeString(id, 'Status item ID')
    assertNativeString(title, 'Status item title')
    if (this._ids.has(id)) throw new Error('Status item IDs must be unique')
    const { enabled = true } = opts
    if (typeof enabled !== 'boolean') throw new TypeError('enabled must be a boolean')

    binding.statusItemAddItem(this._handle, id, title, enabled)
    this._ids.add(id)
    return this
  }

  addSeparator() {
    this._assertLive()
    binding.statusItemAddSeparator(this._handle)
    return this
  }

  updateItem(id, opts = {}) {
    this._assertLive()
    assertNativeString(id, 'Status item ID')
    if (!this._ids.has(id)) throw new Error(`Unknown status item ID: ${id}`)

    const hasTitle = Object.prototype.hasOwnProperty.call(opts, 'title')
    const hasEnabled = Object.prototype.hasOwnProperty.call(opts, 'enabled')
    if (hasTitle) assertNativeString(opts.title, 'title')
    if (hasEnabled && typeof opts.enabled !== 'boolean') {
      throw new TypeError('enabled must be a boolean')
    }

    binding.statusItemUpdateItem(
      this._handle,
      id,
      hasTitle ? opts.title : '',
      hasTitle,
      hasEnabled ? opts.enabled : false,
      hasEnabled
    )
    return this
  }

  destroy() {
    if (this._destroyed) return
    this._destroyed = true
    binding.statusItemDestroy(this._handle)
    this.removeAllListeners()
  }

  _assertLive() {
    if (this._destroyed) throw new Error('StatusItem is destroyed')
  }

  _onselect(id) {
    if (!this._destroyed) this.emit('select', id)
  }
}

function assertNativeString(value, name) {
  if (typeof value !== 'string') throw new TypeError(`${name} must be a string`)
  if (value.includes('\0')) throw new TypeError(`${name} must not contain NUL`)
}
