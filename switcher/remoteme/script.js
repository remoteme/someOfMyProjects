var remoteme;
var operationTimer=  new OperationTimer();

function setup(){
	
	$('#readButton').on('click', function() {
		console.info("read click");
		remoteme.sendUserSyncMessageRest(temperatureArduinoDeviceId,[],onResponse);
	});


	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		automaticlyConnectWebRTC:false,
		webSocketConnectionChange: undefined,
		onUserMessage: onUserMessage,
		webRTCConnectionChange: undefined,
		mediaConstraints: {'mandatory': {'OfferToReceiveAudio': false, 'OfferToReceiveVideo': false}}
	});




	$("#servoSlider" ).slider();
	$("#servoSlider").on("slide", function(slideEvt) {
		operationTimer.execute(sendServo,slideEvt.value);

	});


	$("#ledSlider" ).slider();
	$("#ledSlider").on("slide", function(slideEvt) {
		operationTimer.execute(sendLed,slideEvt.value);

	});


	$('#lcdDisplay').bind('input propertychange', function() {

		var split = this.value.split("\n");
		split[0]=split[0].substring(0,16);

		var spaces = "                ";
		var line1=split[0]+ spaces.substring(split[0].length);
		var line2=spaces;
		if (split.length>1){
			split[1]=split[1].substring(0,16);
			line2=split[1]+ spaces.substring(split[1].length);
		}

		dataToSend = new RemoteMeData(16*2+2+1);
		dataToSend.putByte(1);
		dataToSend.putString(line1);
		dataToSend.putString(line2);
		remoteme.sendUserMessageWebsocket(switcherArduinoDeviceId,dataToSend.getArray());

	});

}

function sendServo(value){
	remoteme.sendUserMessageWebsocket(switcherArduinoDeviceId,[2,value]);
}

function sendLed(value){
	remoteme.sendUserMessageWebsocket(switcherArduinoDeviceId,[3,value]);
}

function onUserMessage(sender,data){
	var data = new RemoteMeData(data);
	console.debug("on data received"+data.getArray());
	var type = data.popUint8();
	if (type==1){//potentiometr
		var value = data.popUint8();
		$('#potentiometr > .progress-bar').css('width',`${100*value/256}%`);
		console.info(`${100*value/256}%`);
	}else if (type==2){
		$('#buttonClickOutput').html(parseInt($('#buttonClickOutput').html())+1);

	}else if (type==3){
		$('#encoderOutput').html(parseInt($('#encoderOutput').html())+ data.popInt8());

	}


}