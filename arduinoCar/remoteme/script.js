var remoteme;
var carController;
var ot;

function setup(){
	ot = new OperationTimer(100);

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



	$('.lightModeButton').on('click',function(event){
		var mode = parseInt($(this).attr("mode"));
		sendLight(mode);


	});

	remoteme.directWebSocketConnectionConnect(arduinoDeviceId,webSocketLocalConnectionChange);//connect just after start


	//---------- setup car

	carController = new CarController4WD();
	carController.ignoreBackwardMode=true;

	new Touch(onMove);

}

function onMove(x,y){
	var height=$(window).height();
	var width=$(window).width();
	var divider=Math.min(height,width)/3.5;
	console.info(divider);
	carController.setTurn(-x/divider);
	carController.setSpeed(y/divider);

	carController.compute();


	var leftHeight = (carController.getLeftSideSpeed()*height/4);
	var rightHeight = (carController.getRightSideSpeed()*height/4);

	$('#leftArrow').css('height',Math.abs(leftHeight)+"px");
	$('#leftArrow').css('top',(height/2) -(leftHeight>0?leftHeight:0)+"px");

	$('#rightArrow').css('height',Math.abs(rightHeight)+"px");
	$('#rightArrow').css('top',(height/2) -(rightHeight>0?rightHeight:0)+"px");
	ot.execute(sendMotor);

}

function fasterStart(x){
	mn=x<0?-1:1;
	return Math.pow(Math.abs(x),0.4)*mn;
}

function sendMotor(){
	var ret = new RemoteMeData(5);


	ret.putInt8(1);
	ret.putInt16(fasterStart(carController.getLeftSideSpeed())*1024);
	ret.putInt16(fasterStart(carController.getRightSideSpeed())*1024 );

	console.info(ret)
	remoteme.sendUserMessageByFasterChannel(arduinoDeviceId,ret);
}

function sendLight(mode){
	var ret = new RemoteMeData(3);


	ret.putInt8(2);
	ret.putInt16(mode );


	remoteme.sendUserMessageByFasterChannel(arduinoDeviceId,ret);
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




