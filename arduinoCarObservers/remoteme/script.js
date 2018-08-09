var remoteme;
function is_touch_device() {
	var prefixes = ' -webkit- -moz- -o- -ms- '.split(' ');
	var mq = function(query) {
		return window.matchMedia(query).matches;
	}

	if (('ontouchstart' in window) || window.DocumentTouch && document instanceof DocumentTouch) {
		return true;
	}

	// include the 'heartz' as a way to have a non matching MQ to help terminate the join
	// https://git.io/vznFH
	var query = ['(', prefixes.join('touch-enabled),('), 'heartz', ')'].join('');
	return mq(query);
}
function setup() {
	remoteme = new RemoteMe();

	remoteme.getObservers().observeBoolean("forward", function (value) {
		$("#forward").css("background-color",value?"green":"white");
	});
	remoteme.getObservers().observeBoolean("back", function (value) {
		$("#back").css("background-color",value?"green":"white");
	});
	remoteme.getObservers().observeBoolean("left", function (value) {
		$("#left").css("background-color",value?"green":"white");
	});

	remoteme.getObservers().observeBoolean("right", function (value) {
		$("#right").css("background-color",value?"green":"white");
	});

	var startEventType = 'mousedown',
		endEventType   = 'mouseup';

	if (is_touch_device()) {
		startEventType = 'touchstart';
		endEventType   = 'touchend';
	}

	$('#forward').on(startEventType, function () {
		remoteme.getObservers().setBoolean("forward",  true);
	});

	$('#back').on(startEventType, function () {
		remoteme.getObservers().setBoolean("back",  true);
	});

	$('#left').on(startEventType, function () {
		remoteme.getObservers().setBoolean("left",  true);
	});

	$('#right').on(startEventType, function () {
		remoteme.getObservers().setBoolean("right",  true);
	});

	$('#forward').on(endEventType, function () {
		remoteme.getObservers().setBoolean("forward",  false);
	});

	$('#back').on(endEventType, function () {
		remoteme.getObservers().setBoolean("back",  false);
	});

	$('#left').on(endEventType, function () {
		remoteme.getObservers().setBoolean("left",  false);
	});

	$('#right').on(endEventType, function () {
		remoteme.getObservers().setBoolean("right",  false);
	});


}

