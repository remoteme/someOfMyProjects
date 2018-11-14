

var ot;

var components=[];

function setupComponents(){

	servos = $( ".servo" ).each(function() {
		servoId=parseInt($(this).attr("servoId"));


		var sliderComponent =$(this).find( ".slider" ).slider();


		var spinnerComponent=$(this).find( ".spinner" );


		sliderComponent.on('slide',onSlide).data({"servoId":servoId});
		spinnerComponent.bind('keyup mouseup',onSpin);

		components[servoId]={spinner:spinnerComponent,
			slider:sliderComponent,value:0};


	});

	ot=new OperationTimer(200);
	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		automaticlyConnectWebRTC:false,
		webSocketConnectionChange: webSocketConnectionChange,
		webRTCConnectionChange: webRtcConnectionChange,
		mediaConstraints: {'mandatory': {'OfferToReceiveAudio': false, 'OfferToReceiveVideo': false}}
	});

	$('#websocketButton').on('click', function() {
		if (remoteme.isWebSocketConnected()) {
			remoteme.disconnectWebSocket();
		}else{
			remoteme.connectWebSocket();
		}

	});
	$('#webRtcButton').on('click', function() {
		if (remoteme.isWebRTCConnected()) {
			remoteme.disconnectWebRTC();
		}else{
			remoteme.connectWebRTC();
		}
	});


}


function onSpin(event){
	var servoId=parseInt($(this.parentElement).attr('servoId'));
	var value=parseInt(components[servoId].spinner[0].value);
	if (isNaN(value) ){
		value=0;
	}
	components[servoId].slider.slider("setValue",value);
	components[servoId].value=value;

	setServo(servoId);
}

function onSlide(event){
	servoId=parseInt($(this.parentElement).attr('servoId'));


	components[servoId].spinner.val(event.value);
	components[servoId].value=event.value;
	setServo(servoId);
}



var timeoutsByServo=[];
function setServo(servoId){

	if  (remoteme.isWebRTCConnected()){
		ot.defaultDelay=150;
	}else{
		ot.defaultDelay=400;
	}
	ot.execute(setServoNow,servoId)


}

function setServoNow(servoId) {
	value=components[servoId].value;
	console.log("setting servo: "+servoId+" "+value);


	var ret = new RemoteMeData(3);


	ret.putByte(servoId );
	ret.putShort(value );


	remoteme.sendUserMessageByFasterChannel(servoPythonDeviceId,ret);
}


function webSocketConnectionChange(state){
	if (state==ConnectingStatusEnum.CONNECTED){
		$("#websocketButton").html("Websocket - connected");
	}else if (state==ConnectingStatusEnum.DISCONNECTED){
		$("#websocketButton").html("Websocket - disconnected");
	}else if (state==ConnectingStatusEnum.ERROR){
		$("#websocketButton").html("Websocket - error");
	}

}




function webRtcConnectionChange(state){

	if (state==ConnectingStatusEnum.CONNECTED) {
		$("#webRtcButton").html("WebRTC - connected");
		$("#webRtcButton").prop("disabled",!true)
	}else if (state==ConnectingStatusEnum.CONNECTING) {
		$("#webRtcButton").html("WebRTC - connecting");
		$("#webRtcButton").prop("disabled",!false);
	}else if (state==ConnectingStatusEnum.DISCONNECTING) {
		$("#webRtcButton").html("WebRTC - disconnecting");
		$("#webRtcButton").prop("disabled",!false);
	}else if (state==ConnectingStatusEnum.CHECKING) {
		$("#webRtcButton").html("WebRTC - checking");
		$("#webRtcButton").prop("disabled",!false);
	}else if (state==ConnectingStatusEnum.DISCONNECTED) {
		$("#webRtcButton").html("WebRTC - disconected");
		$("#webRtcButton").prop("disabled",!true);
	}else if (state==ConnectingStatusEnum.FAILED) {
		$("#webRtcButton").html("WebRTC - failed");
		$("#webRtcButton").prop("disabled",!true);
	}
}



