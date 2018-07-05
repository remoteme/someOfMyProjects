var globalTouch;
class Touch{


	addStyle(){
		var style = document.createElement('style');
		style.type = 'text/css';
		style.innerHTML = '.steer{ position:absolute; opacity: 0.5; width:60px; height:60px; border-radius: 60px; border:1px solid gray; top:calc(100% /2 - 80px);}' +
			'#steer{left:calc(50% - 30px)  } ' ;
		document.getElementsByTagName('head')[0].appendChild(style);

	}
	createDiv(id) {
		jQuery('<div/>', {
			id: id,
			class: 'steer'
		}).appendTo('body');
	}

	constructor(onMove) {
		this.addStyle();
		this.createDiv("steer");
		this.onMove=onMove;


		globalTouch=this;
		$("#out").on('click',function(){$('#out').html("")});
		$(document).on('touchstart',this.touchStart);
		$(document).on('touchend',this.touchEnd);
		$(document).on('touchmove',this.touchmove);
		$(document).on('mouseDown', this.mouseDown);

		this.left={x:0,y:0};


		this.leftDelta={x:0,y:0};

	}

	touchStart(e){
		var pos;
		var div;
		globalTouch.left.x=e.changedTouches[0].pageX;
		globalTouch.left.y=e.changedTouches[0].pageY;
		div=$('#steer');
		globalTouch.onChanged(0,0);

		div.css('top',(e.changedTouches[0].pageY-div.height()/2)+"px");
		div.css('left',(e.changedTouches[0].pageX-div.width()/2)+"px");
		div.css('background-color',"#5aff61");


	}

	touchEnd(e){
		var div;

		div=$('#steer');
		globalTouch.onChanged(0,0);


	//	div.css('display','none');
		div.css('top','');
		div.css('left','');
		div.css('background-color','');

	}

	touchmove(e){
		e.preventDefault();


		var currentLeftDelta={x:globalTouch.leftDelta.x,y:globalTouch.leftDelta.y};

		for(var i=0;i<e.changedTouches.length;i++){
			var current={x:e.changedTouches[i].pageX,y:e.changedTouches[i].pageY};

			var left =current.x<($(document).width()/2);



			currentLeftDelta.x=globalTouch.left.x-current.x;
			currentLeftDelta.y=globalTouch.left.y-current.y;
		}


		if (globalTouch.getCartesianDiff(currentLeftDelta,globalTouch.leftDelta)>10){
			globalTouch.onChanged(currentLeftDelta.x,currentLeftDelta.y);
		}

	}

	getCartesianDiff(p1,p2){
		return Math.sqrt(Math.pow(p1.x-p2.x,2)+Math.pow(p1.y-p2.y,2))
	}

	onChanged(x,y){
		globalTouch.leftDelta.x=x;
		globalTouch.leftDelta.y=y;

		globalTouch.onMove(x,y);

	}



}




