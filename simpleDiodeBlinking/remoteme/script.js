var remoteme;

function setup(){
	remoteme = new RemoteMe();

	remoteme.getObservers().observeBoolean("led",function(value){
		if (value){
			$('.led').addClass( "on" );
		}else{
			$('.led').removeClass( "on" );
		}
	});

	$('.led').on('click', function() {
		remoteme.getObservers().setBoolean("led",!($('.led').hasClass("on")));
	});
}

