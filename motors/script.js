



var components=[];
var ot;

function setupComponents(){
	ot=new OperationTimer(100);

	motors = $( ".motor" ).each(function() {
		motorId=parseInt($(this).attr("motorId"));


		var sliderComponent =$(this).find( ".slider" ).slider({
			formatter: function (value) {
				return 'Current value: ' + value;
			}
		});


		var spinnerComponent=$(this).find( ".spinner" );

		sliderComponent.on('slide',function(event){
			motorId=parseInt($(this.parentElement).attr('motorId'));


			components[motorId].spinner.val(event.value);
			components[motorId].value=event.value;
			setMotor(motorId);
		}).data({"motorId":motorId});

		//spinnerComponent.change(onSpin);


		spinnerComponent.bind('keyup mouseup',function(event){
			var motorId=parseInt($(this.parentElement).attr('motorId'));
			var value=parseInt(components[motorId].spinner[0].value);
			if (isNaN(value) ){
				value=0;
			}
			components[motorId].slider.slider("setValue",value);
			components[motorId].value=value;

			setMotor(motorId);


		});

		components[motorId]={spinner:spinnerComponent,
			slider:sliderComponent,value:0};
	});




	$('.motor .mode').on('click', function() {
		$(this).parent().find("button").removeClass('active');
		$(event.target).addClass('active')
		var motorId=parseInt($(this).parent().attr("motorId"));
		var mode =components[motorId].mode=parseInt($(event.target).attr("mode"));
		components[motorId].mode=mode;

		setMotor(motorId);
	});



	//-------------------


	$('#messageMode button').on('click', function() {
		$('#messageMode button').removeClass('active')
		$(this).addClass('active');


	});


	$('#messageMode button').on('click', function() {
		$('#messageMode button').removeClass('active')
		$(this).addClass('active');
	});




	$('#webSocketState').on('click', function() {
		if (isWebSocketConnected()){
			disconnectWebSocket();
		}else{
			connectWebSocket();
		}

	});
	$('#webRTCState').on('click', function() {
		if (isWebRTCConnected()){
			disconnectWebRTC();
		}else{
			connectWebRTC();
		}
	});



}


function isWebRtc(){
	return $('#messageMode > .active').attr("webrtc");
}





function setMotor(motorId){

	ot.execute("setMotors",setMotorNow,motorId)

}

function setMotorNow(motorId) {
	value = components[motorId].value;
	mode = components[motorId].mode;

	if (mode == undefined) {
		mode=2;
	}


	console.log("setting motor: "+motorId+" mode: "+mode +" value: "+value);


	var ret = new RemoteMeData(4);


	ret.putByte(motorId );
	ret.putByte(mode );
	ret.putShort(value );


	if  (isWebRtc()){
		sendUserMessageWebrtc(444,ret);
	}else{
		sendUserMessageWebsocket(444,ret);

	}
}



function webSocketConnectionChange(state){
	console.info("webosvcket change "+state);
	if (state==ConnectingStatusEnum.CONNECTED){
		$("#webSocketState").removeClass('btn-secondary');
		$("#webSocketState").addClass('btn-success');
	}else if (state==ConnectingStatusEnum.DISCONNECTED  || state==ConnectingStatusEnum.ERROR ){
		$("#webSocketState").removeClass('btn-success');
		$("#webSocketState").addClass('btn-secondary');
	}

}

function webRtcConnectionChange(state){
	if (state){
		$("#webRTCState").removeClass('btn-secondary');
		$("#webRTCState").addClass('btn-success');
	}else{
		$("#webRTCState").removeClass('btn-success');
		$("#webRTCState").addClass('btn-secondary');
	}
}



