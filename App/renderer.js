// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const serialport = require('serialport')
const pkg = require('./package.json')

const $ports = document.getElementById('ports')
const $refreshPorts = document.getElementById('refreshPorts')
const $connect = document.getElementById('connect')
const $error = document.getElementById('error')
const $errorMessage = document.getElementById('error-message')
const $errorHide = document.getElementById('error-hide')

document.title = `${pkg.name} - v${pkg.version}`

$refreshPorts.addEventListener('click', updatePorts, false)
$connect.addEventListener('click', connect, false)
$errorHide.addEventListener('click', hideError, false)

function showError (message) {
  $errorMessage.textContent = message
  $error.style.display = 'block'
}

function hideError () {
  $error.style.display = 'none'
  $errorMessage.textContent = ''
}

hideError()

function updatePorts () {
  serialport.list((err, ports) => {
    console.log('ports', ports)
    hideError()

    if (err) {
      return showError(err.message)
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

  console.log($ports.value)
  showError($ports.value)
}

updatePorts();
