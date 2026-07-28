const EventEmitter = require('bare-events')
const binding = require('../binding')

module.exports = class AppKitStatusItem extends EventEmitter {
  constructor(opts = {}) {
    super()

    const { systemImageName, accessibilityDescription = '' } = opts
    if (typeof systemImageName !== 'string') throw new TypeError('systemImageName must be a string')
    if (typeof accessibilityDescription !== 'string') {
      throw new TypeError('accessibilityDescription must be a string')
    }

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
    if (typeof id !== 'string' || typeof title !== 'string') {
      throw new TypeError('Status item IDs and titles must be strings')
    }
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
    if (typeof id !== 'string') throw new TypeError('Status item ID must be a string')
    if (!this._ids.has(id)) throw new Error(`Unknown status item ID: ${id}`)

    const hasTitle = Object.prototype.hasOwnProperty.call(opts, 'title')
    const hasEnabled = Object.prototype.hasOwnProperty.call(opts, 'enabled')
    if (hasTitle && typeof opts.title !== 'string') throw new TypeError('title must be a string')
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
