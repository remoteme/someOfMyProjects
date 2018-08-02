var remoteme;

function setup(){
	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		webSocketConnectionChange: webSocketConnectionChange
	});

	$('#webSocketState').on('click', function() {
		if (remoteme.isWebSocketConnected()){
			remoteme.disconnectWebSocket();
		}else{
			remoteme.connectWebSocket();
		}

	});
	$('#directWebSocketConnection').on('click', function() {
		if (remoteme.isDirectWebSocketConnectionConnected(arduinoDeviceId)){
			remoteme.directWebSocketConnectionDisconnect(arduinoDeviceId);
		}else{
			remoteme.directWebSocketConnectionConnect(arduinoDeviceId,webSocketLocalConnectionChange);
		}

	});



	remoteme.getVariablesObserver().observe("motor",VariableOberverType.INTEGER,function(value){
		console.info("changed "+value);
	});
	remoteme.getVariablesObserver().observe("led",VariableOberverType.BOOLEAN,function(value){
		if (value){
			$('.led').addClass( "on" );
		}else{
			$('.led').removeClass( "on" );
		}
	});

	$('.led').on('click', function() {
		remoteme.getVariablesObserver().setBoolean("led",!($('.led').hasClass("on")));
	});
}





function webSocketLocalConnectionChange(deviceId,state){
	$("#directWebSocketConnection").removeClass('btn-secondary');
	$("#directWebSocketConnection").removeClass('btn-success');
	$("#directWebSocketConnection").removeClass('btn-danger');

	if (state==WebsocketConnectingStatusEnum.CONNECTED) {
		$("#directWebSocketConnection").addClass('btn-success');
	}else if (state==WebsocketConnectingStatusEnum.ERROR){
		$("#directWebSocketConnection").addClass('btn-danger');
	}else if (state==WebsocketConnectingStatusEnum.DISCONNECTED){
		$("#directWebSocketConnection").addClass('btn-secondary');
	}
}

function webSocketConnectionChange(state){
	$("#webSocketState").removeClass('btn-secondary');
	$("#webSocketState").removeClass('btn-success');
	$("#webSocketState").removeClass('btn-danger');

	if (state==WebsocketConnectingStatusEnum.CONNECTED) {
		$("#webSocketState").addClass('btn-success');


	}else if (state==WebsocketConnectingStatusEnum.ERROR){
		$("#webSocketState").addClass('btn-danger');
	}else if (state==WebsocketConnectingStatusEnum.DISCONNECTED ){
		$("#webSocketState").addClass('btn-secondary');
	}
}





