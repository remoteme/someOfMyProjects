
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
		data:'{ "niceName": "someName", "smartPhoneId": "someId"}',
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

	
   
     $('#createVariables').on('click', function() {
        var toSend=`{
              "type" : "CreateVariablesMessage",
              "variables" : [ {
                "name" : "gamepad",
                "type" : "TEXT_2",
                "persistent" : false,
                "history" : false
              }, {
                "name" : "button1",
                "type" : "BOOLEAN",
                "persistent" : true,
                "history" : false
              }, {
                "name" : "button2",
                "type" : "BOOLEAN",
                "persistent" : true,
                "history" : false
              }, {
                "name" : "button3",
                "type" : "BOOLEAN",
                "persistent" : true,
                "history" : false
              }, {
                "name" : "relay1",
                "type" : "BOOLEAN",
                "persistent" : true,
                "history" : false
              }, {
                "name" : "relay2",
                "type" : "BOOLEAN",
                "persistent" : true,
                "history" : false
              }, {
                "name" : "temperature",
                "type" : "DOUBLE",
                "persistent" : true,
                "history" : true
              } ]
            }
            
            `
		send(toSend)

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
            "name" : "relay1",
            "type" : "BOOLEAN"
          },{
            "name" : "relay2",
            "type" : "BOOLEAN"
          }]
        }
`
		send(toSend)

	});
	
	
	 $('#rename').on('click', function() {
        var toSend=`{
              "type" : "VariableRenameMessage",
              "deviceId" :  ${arLiteId},
              "data" : {
                "oldName" : "button3",
                "newName" : "button35",
                "type" : "BOOLEAN"
              }
            }
`
	send(toSend)

	});
	 $('#delete').on('click', function() {
        var toSend=`{
              "type" : "VariableRemoveMessage",
              "deviceId" :  ${arLiteId},
              "data" : {
                "name" : "button2",
                "type" : "BOOLEAN"
              }
            }
            `
		send(toSend)

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
	send(toSend)

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
		send(toSend)

	});
	
	$('#gamePad').on('click', function() {
        var toSend=`{
            "type" : "VariableChangeMessage",
            "senderDeviceId" :  ${arLiteId},
            "ignoreReceivers" : [ ${arLiteId}],
            "states" : [
                {
                    "type" : "TEXT_2",
                    "name" : "GamePad",
                    "data" : {
                      "text1" : "buttonUp",
                      "text2" : "wes"
                    }
                  } 
            ]
        }
`
		send(toSend)

	});

   $('#sendPersistence').on('click', function() {
       

        var toSend=` {
          "type" : "NotifyAboutVariablesMessage",
          "identifiers" : [ {
            "name" : "button1",
            "type" : "BOOLEAN"
          },{
            "name" : "button2",
            "type" : "BOOLEAN"
          },{
            "name" : "button3",
            "type" : "BOOLEAN"
          },{
            "name" : "relay1",
            "type" : "BOOLEAN"
          },{
            "name" : "relay2",
            "type" : "BOOLEAN"
          }]
        }
`
		send(toSend)

	});

}



function send(toSend){
    
 if ($('#restSend').is(':checked'))  {
    $.ajax({
		url: `http${s}://${URL}/arLite/rest/v1/sendMessage/`,
		contentType: 'application/json',
		data:toSend,
		processData: false,
		type: 'PUT',
	})
		.done(function( data ) {
			deviceId=data;
			$('#deviceId').html(deviceId);
			arLiteId=deviceId;
		});
 }else{
     	webSocket.send(toSend)
 }

}