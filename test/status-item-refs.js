const binding = require('./addon/binding')

let selections = 0
const handle = binding.statusItemInit('network', 'Kepos', {}, () => {
  selections++
})
binding.statusItemAddItem(handle, 'open', 'Open', true)

assert(binding.statusItemTestingState() === 31, 'references were not live before prepare')
assert(binding.statusItemTestingDeleteCount() === 0, 'references deleted too early')
binding.statusItemTestingPrepare()
assert(binding.statusItemTestingState() === 0, 'native callbacks remained after prepare')
assert(binding.statusItemTestingDeleteCount() === 2, 'references were not deleted exactly once')
binding.statusItemTestingPrepare()
binding.statusItemTestingLateSelect()
assert(binding.statusItemTestingDeleteCount() === 2, 'repeat prepare deleted references')
assert(selections === 0, 'late selection entered JavaScript')
binding.statusItemDestroy(handle)
binding.statusItemDestroy(handle)
console.log('status-item-refs: ok')
Bare.exit(0)

function assert(condition, message) {
  if (!condition) throw new Error(message)
}
