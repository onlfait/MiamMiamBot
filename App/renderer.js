// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const serialport = require('serialport')
const pkg = require('./package.json')

const $ports = document.getElementById('ports')
const $refreshPorts = document.getElementById('refreshPorts')
const $connect = document.getElementById('connect')

document.title = `${pkg.name} - v${pkg.version}`

$refreshPorts.addEventListener('click', updatePorts, false)
$connect.addEventListener('click', connect, false)

function updatePorts () {
  serialport.list((err, ports) => {
    console.log('ports', ports);

    if (err) {
      document.getElementById('error').textContent = err.message
      return
    } else {
      document.getElementById('error').textContent = ''
    }

    if (ports.length === 0) {
      $ports.textContent = ''
      addPort('null', 'Aucun port trouvé...')
    } else {
      setPorts(ports)
    }
  })
}

function addPort (value, label) {
  let $option = document.createElement('option')
  $option.textContent = label || value
  $option.value = value
  $ports.appendChild($option)
}

function addPorts (ports) {
  ports.forEach(port => {
    addPort(port.comName, `${port.comName} - ${port.manufacturer}`)
  })
}

function setPorts (ports) {
  $ports.textContent = ''
  addPorts(ports)
}

function connect () {
  let port = $ports.value

  if (port === 'null') {
    return
  }

  console.log($ports.value);
}

updatePorts();
