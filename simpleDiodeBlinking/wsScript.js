
var URL="127.0.0.1:8082";
var token;
var deviceId=undefined;
var websocket;

function getToken(){
	$.ajax({
		url: `http://${URL}/arLite/rest/v1/getToken/`,
		beforeSend: function( xhr ) {
			xhr.overrideMimeType( "text/plain; charset=x-user-defined" );
		}
	})
		.done(function( data ) {
			token=data;
			$('#token').html(token);
		});
}

function register(){
	$.ajax({
		url: `http://${URL}/arLite/rest/v1/register/`,
		contentType: 'application/json',
		data:'{ "niceName": "someName", "smartPhoneId": "someId"}',
		processData: false,
		type: 'POST',
	})
		.done(function( data ) {
			deviceId=data;
			$('#deviceId').html(deviceId);
		});
}

function disconnect(){

}

function connect(){
	if (deviceId==undefined){
		alert('click register first');
	}else{
		webSocket = new WebSocket(`ws://${URL}/arLite/${token}/ws/v1/${deviceId}/`);//or replace with wss

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


}



