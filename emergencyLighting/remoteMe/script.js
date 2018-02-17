var topMode=0;
var bottomMode=0;

function setup(){
	remoteme = new RemoteMe({
		automaticlyConnectWS: false,
		automaticlyConnectWebRTC:false,
		webSocketConnectionChange: undefined,
		webRTCConnectionChange: undefined,
		mediaConstraints: {'mandatory': {'OfferToReceiveAudio': false, 'OfferToReceiveVideo': false}}
	});



	$('#top').on('click', function() {
		$('#top').find("button").removeClass('active');
		$(event.target).addClass('active');
		topMode=parseInt($(event.target).attr("mode"));

		setModes();
	});

	$('#bottom').on('click', function() {
		$('#bottom').find("button").removeClass('active');
		$(event.target).addClass('active');

		bottomMode=parseInt($(event.target).attr("mode"));

		setModes();
	});
}

function setModes(){
	console.info(topMode+" "+bottomMode);
	remoteme.sendUserMessageRest(arduinoLigtindDeviceId,[topMode,bottomMode]);
}



