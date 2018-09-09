
var URL="app.remoteme.org";
var s="s";
var token;
var deviceId=undefined;
var websocket;
var arLiteId;


function getToken(){
	$.ajax({
		url: `http${s}://${URL}/arLite/rest/v1/getToken/`,
		contentType: 'application/json',
		type: 'GET',
	})
		.done(function( data ) {
			token=data.token;
			$('#token').html(token);
		});
}

function register(){
	$.ajax({
		url: `http${s}://${URL}/arLite/rest/v1/register/`,
		contentType: 'application/json',
		data:'{ "niceName": "arduinoFake", "smartPhoneId": "someIdXX"}',
		type: 'POST',
	})
		.done(function( data ) {
			deviceId=data.deviceId;
			$('#deviceId').html(deviceId);
			arLiteId=deviceId;
		});
}


function disconnect(){

}

function connect(){
	if (deviceId==undefined){
		alert('click register first');
	}else{
		webSocket = new WebSocket(`ws${s}://${URL}/arLite/${token}/ws/v1/${deviceId}/`);//or replace with wss

		webSocket.onmessage = function (event) {
			$('#incommingMessage').val(event.data);
		};

		setInterval(function(){
			console.info("ping sent");
			webSocket.send('{  "type" : "PingMessage"}');

			},60000);


	}

}
function setup(){
	getToken();

	$('#register').on('click', function() {
		register();

	});

	$('#connect').on('click', function() {
		connect();
	});
	$('#disconnect').on('click', function() {
		webSocket.close();

	});

	$('#send').on('click', function() {
		webSocket.send($("#toSend").val())

	});


    $('#observeVariables').on('click', function() {
        var toSend=`{
          "type" : "VariableObserveMessage",
          "deviceId" : ${arLiteId},
          "variables" : [ {
            "name" : "button1",
            "type" : "BOOLEAN"
          },{
            "name" : "button2",
            "type" : "BOOLEAN"
          },{
            "name" : "button3",
            "type" : "BOOLEAN"
          },{
            "name" : "buttonChange",
            "type" : "BOOLEAN"
          },{
            "name" : "relay1",
            "type" : "BOOLEAN"
          },{
            "name" : "relay2",
            "type" : "BOOLEAN"
          }, {
            "name" : "GamePad",
            "type" : "TEXT_2"
          } ]
        }
`
		webSocket.send(toSend)

	});
	
	
	
		 $('#button1True').on('click', function() {
        var toSend=`{
            "type" : "VariableChangeMessage",
            "senderDeviceId" :  ${arLiteId},
            "ignoreReceivers" : [ ${arLiteId}],
            "states" : [
                {
                    "type" : "BOOLEAN",
                    "name" : "button1",
                    "data" : true
                }]
        }
`
		webSocket.send(toSend)

	});


	$('#button1False').on('click', function() {
        var toSend=`{
            "type" : "VariableChangeMessage",
            "senderDeviceId" :  ${arLiteId},
            "ignoreReceivers" : [ ${arLiteId}],
            "states" : [
                {
                    "type" : "BOOLEAN",
                    "name" : "button1",
                    "data" : false
                }]
        }
`
		webSocket.send(toSend)

	});
	

}



