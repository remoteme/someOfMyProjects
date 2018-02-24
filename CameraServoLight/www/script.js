var ot=new OperationTimer(150);

function onMouseMove(event){
   positionChange(event.x ,event.y);
}

function onTouchMove(event){
   positionChange(event.touches[0].clientX , event.touches[0].clientY);
}

function positionChange(x,y){
     x=(x/$("#remoteVideo").width()-0.5)*2;
     y=(y/$("#remoteVideo").height()-0.5)*2;   
  
     ot.execute(sendCameraNow,x,y);
      
}

function sendCameraNow(x,y){
    $("#output").html(x+" - "+y);
    
    var ret = new Uint8Array(5);
	var pos=0;

    pos=putByte(ret, pos ,1 );
	pos=putShort(ret, pos ,568-x*312 );
	pos=putShort(ret, pos ,506+y*237 );
	
   remoteme.sendUserMessageByFasterChannel(servoPythonScriptDeviceId, ret);
}

function diodeClick(){

    
	var nextState;
	if ($("#diode").hasClass("diodeOn")){
		$("#diode").removeClass( "diodeOn" );
		nextState=0;
	}else{
		$("#diode").addClass( "diodeOn" );
		nextState=1;
	}


    var ret = new Uint8Array(5);
    var pos=0;
    pos=putByte(ret, pos ,2 );
    pos=putShort(ret, pos ,nextState );
    remoteme.sendUserMessageByFasterChannel(servoPythonScriptDeviceId,ret);

}

var remoteme;

function setupComponents(){
	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		automaticlyConnectWebRTC:true,
		webSocketConnectionChange: webSocketConnectionChange,
		webRTCConnectionChange: webRtcConnectionChange,
	});


}


function webSocketConnectionChange(state){
	console.info("webosvcket change "+state);

}

function webRtcConnectionChange(state){
	if (state==WebrtcConnectingStatusEnum.CONNECTED){
		$("#webRTCState").removeClass('btn-secondary');
		$("#webRTCState").addClass('btn-success');


	}else{
		$("#webRTCState").removeClass('btn-success');
		$("#webRTCState").addClass('btn-secondary');
	}

	if (state==WebrtcConnectingStatusEnum.CONNECTED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Connected");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}else if (state==WebrtcConnectingStatusEnum.CONNECTING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Connecting");
	}else if (state==WebrtcConnectingStatusEnum.DISCONNECTING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Disconnecting");
	}else if (state==WebrtcConnectingStatusEnum.CHECKING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Checking");
	}else if (state==WebrtcConnectingStatusEnum.DISCONNECTED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Disconnected");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}else if (state==WebrtcConnectingStatusEnum.FAILED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Failed");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}
}



function fullscreen() {
	var isInFullScreen = (document.fullscreenElement && document.fullscreenElement !== null) ||
		(document.webkitFullscreenElement && document.webkitFullscreenElement !== null) ||
		(document.mozFullScreenElement && document.mozFullScreenElement !== null) ||
		(document.msFullscreenElement && document.msFullscreenElement !== null);

	var docElm = document.documentElement;
	if (!isInFullScreen) {
		if (docElm.requestFullscreen) {
			docElm.requestFullscreen();
		} else if (docElm.mozRequestFullScreen) {
			docElm.mozRequestFullScreen();
		} else if (docElm.webkitRequestFullScreen) {
			docElm.webkitRequestFullScreen();
		} else if (docElm.msRequestFullscreen) {
			docElm.msRequestFullscreen();
		}
	} else {
		if (document.exitFullscreen) {
			document.exitFullscreen();
		} else if (document.webkitExitFullscreen) {
			document.webkitExitFullscreen();
		} else if (document.mozCancelFullScreen) {
			document.mozCancelFullScreen();
		} else if (document.msExitFullscreen) {
			document.msExitFullscreen();
		}
	}
}