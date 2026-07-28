# bare-app-kit

AppKit bindings and runtime for Bare.

```
npm i bare-app-kit
```

## Status items

```js
const AppKit = require('bare-app-kit')

const item = new AppKit.StatusItem({
  systemImageName: 'network',
  accessibilityDescription: 'Network status'
})

item.addItem('status', 'Starting', { enabled: false })
item.addSeparator()
item.addItem('quit', 'Quit')
item.on('select', (id) => console.log(id))
item.updateItem('status', { title: 'Online' })
item.destroy()
```

Set `Window.hidesOnClose` to opt into red-button close-to-hide. `hide()`
orders the window out, while `show()` activates the app and restores it.
Programmatic `close()` remains terminal.

The runtime posts `BareApplicationWillTerminateNotification` synchronously
before tearing down Bare. Native callback owners remove targets and delete
JavaScript references while the environment is still live.

## License

Apache-2.0
