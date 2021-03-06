// -----------------------------------------------------------------------------
// Titre de la fenêtre
// -----------------------------------------------------------------------------
const version = require('../package').version
const title = `MiamMiamBot App v${version}`
document.title = title
console.log(title)

// -----------------------------------------------------------------------------
// Erreurs
// -----------------------------------------------------------------------------
const $error = document.getElementById('error')
const $errorClose = $error.querySelector('.message-header button.delete')
const $errorMessage = $error.querySelector('.message-body')

$errorClose.addEventListener('click', hideError, false)

function showError (message, timeout = 5000) {
  $errorMessage.innerText = message
  $error.classList.remove('is-hidden')
  timeout && setTimeout(hideError, timeout)
}

function hideError () {
  $error.classList.add('is-hidden')
  $errorMessage.innerText = ''
}

// -----------------------------------------------------------------------------
// Connection
// -----------------------------------------------------------------------------
const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const $connected = document.getElementById('connected')
const $connection = document.getElementById('connection')
const $connecting = document.getElementById('connecting')
const $connectButton = $connection.querySelector('.connect.button')

$connectButton.addEventListener('click', connect, false)

let connected = false
let connecting = false
let port = null

function onConnectSuccess () {
  console.log('onConnectSuccess')
  connected = true
  connecting = false
  $connecting.classList.add('is-hidden')
  $connected.classList.remove('is-hidden')
}

function onConnectError (error=null) {
  console.log('onConnectError')
  error && console.error(error)
  port = null
  connected = false
  connecting = false
  showError('MiamMiamBot introuvable')
  $connected.classList.add('is-hidden')
  $connecting.classList.add('is-hidden')
  $connection.classList.remove('is-hidden')
}

function getPorts () {
  return new Promise((resolve, reject) => {
    SerialPort.list((error, ports) => {
      if (error) {
        return reject(error)
      }
      if (ports.length === 0) {
        return reject(new Error('Aucun port trouvé.'))
      }
      resolve(ports)
    })
  })
}

function onData (data) {
  let args = data.split('|')
  let cmd = args.shift()
  if (cmd === 'dateTime') {
    setDateTime(args)
  } else if (cmd === 'alarm') {
    setAlarm(args)
  } else if (cmd === 'motor') {
    setMotor(args)
  } else if (cmd === 'ok') {
    console.log('ok')
  } else {
    console.log('undefined command:', { cmd, args })
  }
}

function connectPort (comName, baudRate=115200, timeout=5000) {
  console.log('connecting:', comName)
  return new Promise((resolve, reject) => {
    port = SerialPort(comName, { baudRate, autoOpen: false })
    const parser = port.pipe(new Readline({ delimiter: '\n' }))
    parser.on('data', data => {
      if (data.startsWith('MiamMiamBot|')) {
        parser.on('data', onData)
        connected = true
        connecting = false
        getRemoteDateTime()
        getRemoteAlarms()
        getRemoteMotor()
        resolve(port)
      } else {
        reject(new Error('Bad fingerprint'))
      }
    })
    port.on('error', reject)
    port.on('close', onConnectError)
    port.open()
    setTimeout(function () {
      if (!connected) {
        reject(new Error('Bad fingerprint'))
      }
    }, timeout)
  })
}

function _connectPort (ports, resolve, reject) {
  connectPort(ports.shift().comName).then(resolve).catch(error => {
    if (ports.length) {
      _connectPort(ports, resolve, reject)
    } else {
      reject(error)
    }
  })
}

function _connect () {
  return new Promise((resolve, reject) => {
    getPorts().then(ports => {
      _connectPort(ports, resolve, reject)
    }).catch(reject)
  })
}

function connect () {
  if (connected || connecting) {
    return
  }
  hideError()
  connecting = true
  $connection.classList.add('is-hidden')
  $connecting.classList.remove('is-hidden')
  _connect().then(onConnectSuccess).catch(onConnectError)
}

// -----------------------------------------------------------------------------
// Horloge
// -----------------------------------------------------------------------------
const $clock = document.getElementById('clock')
const $clockDate = $clock.querySelector('.date')
const $clockTime = $clock.querySelector('.time')
const $clockUpdate = $clock.querySelector('.update')

const twoDigits = x => x < 10 ? `0${x}` : x

let clockTimeout = null

$clockUpdate.addEventListener('click', updateRemoteDateTime, false)

function setDateTime (args) {
  const [ D, M, Y, h, m, s ] = args.map(twoDigits)
  $clockDate.innerText = `${D}/${M}/${Y}`
  $clockTime.innerText = `${h}:${m}:${s}`
}

function setRemoteDateTime (args) {
  if (!port) return
  // setDateTime|dd|mm|yyyy|hh|mm|ss
  const cmd = `setTime|${args.join('|')}\n`
  console.log(cmd)
  port.write(cmd)
}

function updateRemoteDateTime () {
  const d = new Date()
  setRemoteDateTime([
    d.getDate(),
    d.getMonth() + 1,
    d.getFullYear(),
    d.getHours(),
    d.getMinutes(),
    d.getSeconds()
  ])
}

function getRemoteDateTime () {
  if (!port) return
  port.write(`getTime\n`)
  clockTimeout = setTimeout(getRemoteDateTime, 1000)
}

// -----------------------------------------------------------------------------
// Template
// -----------------------------------------------------------------------------
const $template = document.querySelector('template').content
const $alarmTemplate = $template.querySelector('.alarm')

function appendTemplate ($tpl, $target) {
  const $node = $tpl.cloneNode(true)
  $target.appendChild($node)
  return $node
}

// -----------------------------------------------------------------------------
// Alarms
// -----------------------------------------------------------------------------
const $alarms = document.getElementById('alarms')
const $alarmslist = $alarms.querySelector('.alarms')
const $alarmsUpdate = $alarms.querySelector('.update')
const $alarmsDivs = new Map()
const alarmsCount = 5

$alarmsUpdate.addEventListener('click', updateRemoteAlarms, false)

function appendAlarm (i) {
  const $alarm = appendTemplate($alarmTemplate, $alarmslist)
  const $icon = $alarm.querySelector('.icon i')
  const $hour = $alarm.querySelector('.hour')
  const $minute = $alarm.querySelector('.minute')
  const $quantity = $alarm.querySelector('.quantity')
  $alarmsDivs.set(i, [ $alarm, $icon, $hour, $minute, $quantity ])
}

function setAlarm (args) {
  const [ i, hour, minute, quantity ] = args.map(parseFloat)
  const [ $alarm, $icon, $hour, $minute, $quantity ] = $alarmsDivs.get(i)
  $hour.value = hour
  $minute.value = minute
  $quantity.value = quantity
  if (quantity) {
    $icon.classList.add('fa-bell')
    $icon.classList.remove('fa-bell-slash')
  } else {
    $icon.classList.add('fa-bell-slash')
    $icon.classList.remove('fa-bell')
  }
}

function setRemoteAlarm (args) {
  if (!port) return
  // setAlarm|index|heure|minute|quantity
  const cmd = `setAlarm|${args.join('|')}\n`
  console.log(cmd)
  port.write(cmd)
}

function saveRemotSettings () {
  if (!port) return
  console.log('save')
  port.write('save\n')
}

function updateRemoteAlarms (save=true) {
  for (var i = 0; i < alarmsCount; i++) {
    const [ $alarm, $icon, $hour, $minute, $quantity ] = $alarmsDivs.get(i)
    setRemoteAlarm([i, $hour.value, $minute.value, $quantity.value])
  }
  save && saveRemotSettings()
  getRemoteAlarms()
}

function getRemoteAlarms () {
  if (!port) return
  for (var i = 0; i < alarmsCount; i++) {
    port.write(`getAlarm|${i}\n`)
  }
}

for (var i = 0; i < alarmsCount; i++) {
  appendAlarm(i)
  setAlarm([i, null, null, null])
}

// -----------------------------------------------------------------------------
// Nourrir
// -----------------------------------------------------------------------------
const $feed = document.getElementById('feed')
const $feedQuantity = $feed.querySelector('.quantity')
const $feedButton = $feed.querySelector('.feed')

$feedButton.addEventListener('click', feed, false)

function feedRemote (quantity) {
  if (!port) return
  // feed|quantity
  const cmd = `feed|${quantity}\n`
  console.log(cmd)
  port.write(cmd)
}

function feed () {
  feedRemote($feedQuantity.value)
}

// -----------------------------------------------------------------------------
// Moteur
// -----------------------------------------------------------------------------
const $motor = document.getElementById('motor')
const $motorSteps = $motor.querySelector('.steps')
const $motorMaxSpeed = $motor.querySelector('.maxSpeed')
const $motorAcceleration = $motor.querySelector('.acceleration')
const $motorMiscroStepping = $motor.querySelector('.microstepping')
const $motorInvertDirPin = $motor.querySelector('.invertDirPin')
const $motorInvertStepPin = $motor.querySelector('.invertStepPin')
const $motorInvertEnPin = $motor.querySelector('.invertEnPin')
const $motorInvertDir = $motor.querySelector('.invertDir')
const $motorUpdate = $motor.querySelector('.update')

$motorUpdate.addEventListener('click', updateRemoteMotor, false)

function setMotor (args) {
  const [
    steps,
    maxSpeed,
    acceleration,
    microstepping,
    invertDirPin,
    invertStepPin,
    invertEnPin,
    invertDir
  ] = args.map(parseFloat)
  $motorSteps.value = steps
  $motorMaxSpeed.value = maxSpeed
  $motorAcceleration.value = acceleration
  $motorMiscroStepping.value = microstepping
  $motorInvertDirPin.value = invertDirPin
  $motorInvertStepPin.value = invertStepPin
  $motorInvertEnPin.value = invertEnPin
  $motorInvertDir.value = invertDir
}

function getRemoteMotor () {
  if (!port) return
  console.log(`getMotor\n`)
  port.write(`getMotor\n`)
}

function setRemoteMotor (args) {
  if (!port) return
  // setMotor|steps|maxSpeed|acceleration|microstepping|invertDirPin|invertStepPin|invertEnPin|invertDir
  const cmd = `setMotor|${args.join('|')}\n`
  console.log(cmd)
  port.write(cmd)
}

function updateRemoteMotor (save=true) {
  setRemoteMotor([
    $motorSteps.value,
    $motorMaxSpeed.value,
    $motorAcceleration.value,
    $motorMiscroStepping.value,
    $motorInvertDirPin.value,
    $motorInvertStepPin.value,
    $motorInvertEnPin.value,
    $motorInvertDir.value
  ])
  save && saveRemotSettings()
}

// -----------------------------------------------------------------------------
// Inputs
// -----------------------------------------------------------------------------
document.querySelectorAll('input[min], input[max]').forEach($input => {
  $input.addEventListener('change', e => {
    const min = parseInt($input.getAttribute('min'))
    const max = parseInt($input.getAttribute('max'))
    const val = parseInt($input.value)
    if (val > max) {
      $input.value = max
    } else if (val < min) {
      $input.value = min
    }
  })
})
