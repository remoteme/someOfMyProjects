var remoteme;

function setup(){
	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		automaticlyConnectWebRTC:false,
		webSocketConnectionChange: webSocketConnectionChange,
		webRTCConnectionChange: undefined,
		onUserMessage: onUserMessage,
		mediaConstraints: {'mandatory': {'OfferToReceiveAudio': false, 'OfferToReceiveVideo': false}}
	});
}


function onUserMessage(sender,data){
	var data = new RemoteMeData(data);
	changeDiode(data.popUint8())
}

function changeDiode(state){
	$("#led").css("background-color",state==0?"white":"red");
}

function webSocketConnectionChange(state){

	if (state==WebsocketConnectingStatusEnum.CONNECTED){
		remoteme.sendUserSyncMessageWebSocket(arduinoId,[],function(data){
			var data = new RemoteMeData(data);
			changeDiode(data.popUint8());
		});
	}
}

function onLedClick(){
	remoteme.sendUserMessageByFasterChannel (arduinoId,[]);

}