// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.
//require('bulma/css/bulma.css')

const serialport = require('serialport')
const pkg = require('./package.json')

const $ports = document.getElementById('ports')
const $refreshPorts = document.getElementById('refreshPorts')

document.title = `${pkg.name} - v${pkg.version}`

$refreshPorts.addEventListener('click', updatePorts, false)

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
      document.getElementById('error').textContent = 'Pas de ports'
    }

    $ports.textContent = ''

    ports.forEach(port => {
      let $option = document.createElement('option')
      $option.textContent = `${port.comName} - ${port.manufacturer}`
      $option.value = port.comName
      $option.title = port.comName
      $ports.appendChild($option)
      //document.getElementById('ports').innerHTML += port.comName + '<br />'
    })
  })
}

updatePorts();
