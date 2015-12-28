package {
	
	
	import flash.text.TextField;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.*;
	import caurina.transitions.Tweener;

	
	
	public class DropDown extends MovieClip {
		
		
		//Event
		
		static public var ITEM_SELECTED:String = "itemSelected";
		
		//Array
		
		public var dropDownArray:Array = [];
		
		//Item
		
		private var listHolder:Sprite;
		
		private var listScroller:Sprite;
		
		private var masker:Sprite;
		
		private var listBg:Sprite;
		
		private var listHeight:Number;
		
		private var listOpen:Boolean = false;
		
		public var items = [];
		
		public var selectedId:int;
		
		public var selectedLabel:String;
		
		public var selectedData:*;
		
		
		
		
		public function DropDown(_array:Array, _message:String = "Please choose...", _listHeight:Number = 100) {
			
			dropDownArray = _array;
			
			listHeight = _listHeight;
			
			selectedItem_txt.text = _message;
			
			addEventListener(Event.ADDED_TO_STAGE, init);
			
		}
		
		private function init(e:Event):void{
			
			//Add function to hit area
			
			hit.addEventListener(MouseEvent.CLICK, openClose);
			hit.addEventListener(MouseEvent.ROLL_OVER, onTab);
			hit.addEventListener(MouseEvent.ROLL_OUT, outTab);
			
			hit.buttonMode = true;
			
			stage.addEventListener(MouseEvent.CLICK, stageClose);
			
			buildList();
			
		}
		
		private function buildList():void{
			
			listHolder = new Sprite();
			
			addChildAt(listHolder, 0);
			
			
			
			//list scroller
			
			listScroller = new Sprite();
			
			listHolder.addChildAt(listScroller, 0);
			
			listHolder.y = 30;
			
			//Build list
			
			for (var i:int = 0; i < dropDownArray.length; i ++){
				
				var listItem:DropDownItem = new DropDownItem();
				items.push(listItem);
				listItem.name="ddItem";
				listItem._id = i;
				listItem.gotoAndStop(1);

				
				listItem.item_txt.text = dropDownArray[i].label;
				
				listItem._data = dropDownArray[i].data;
				
				listItem.y = listScroller.height;
				
				listItem.mouseChildren = false;
				
				listItem.buttonMode = true;
				
				
				listItem.addEventListener(MouseEvent.MOUSE_OVER, itemOver);
				
				listItem.addEventListener(MouseEvent.MOUSE_OUT, itemOut);
				
				listItem.addEventListener(MouseEvent.CLICK, selectItem);
				
				listScroller.addChild(listItem);
				
			}
			
			if(listScroller.height < listHeight){
				
				listHeight = listScroller.height;
				
			}
			
			//bg
			
			listBg = new Sprite();
			
			//listBg.graphics.beginFill(0x000000);
			listBg.graphics.drawRoundRect(0, -10, listScroller.width, listScroller.height + 10, 11);
			listBg.graphics.endFill();
			
			listScroller.addChildAt(listBg, 0);
			
			//listBg.y = - listHeight;
			
			
			
			//mask
			
			masker = new Sprite();
			
			masker.graphics.beginFill(0x000000);
			masker.graphics.drawRoundRect(0, 0, listScroller.width, listHolder.height-10, 11);
			masker.graphics.endFill();
			
			addChild(masker);
			
			masker.y = 30;
			listHolder.mask = masker;
			listHolder.y=-listHolder.height;
			
		}
		
		public function openClose(e:Event = null):void{
			
			if(listOpen){
				
				close();
				
			} else {
				
				open();
				
			}
			
		}
		
		public function stageClose(e:Event):void{

			if(e.target != this.hit && e.target.name!="ddItem"){
				close();
				
			}
			
		}
		
		//OPEN LIST
		
		public function open(e:Event = null):void{
			
			listOpen = true;
			Tweener.addTween(listHolder,{y:30,time:0.3,transition:"easeOut"});
			
		}
		
		//CLOSE LIST
		
		public function close(e:Event = null):void{

			listOpen = false;
			Tweener.addTween(listHolder,{y:-listHolder.height,time:0.5,transition:"easeOut"});
			
		}
		
		
		
		//SELECTED
		
		private function itemOver(e:Event):void{
			Tweener.addTween(e.target,{alpha:0.8,time:0.5,transition:"easeOut"});
			
			
		}
		
		private function itemOut(e:Event):void{
			Tweener.addTween(e.target,{alpha:1,time:0.5,transition:"easeOut"});
			
		}
		private function onTab(e:Event):void{
			skin.gotoAndStop(2);
			
		}
		private function outTab(e:Event):void{
			skin.gotoAndStop(1);
			
		}
		
		private function selectItem(e:Event):void{
			
			selectedId = e.target._id;
			selectedLabel = e.target.item_txt.text;
			selectedData = e.target._data;
			dispatchEvent( new DropDownEvent(ITEM_SELECTED, selectedId, selectedLabel, selectedData));
			close();
			
		}
		private function listScroll(e:Event):void{
			
			var scrollDistance:int = masker.height;
			
			if(listOpen){
			
				if(listScroller.height > scrollDistance){
					
					listScroller.y += Math.cos((-masker.mouseY/scrollDistance)*Math.PI) * 3;
					
					if (listScroller.y>0) {
						listScroller.y = 0;
					}
					if (-listScroller.y>(listScroller.height-scrollDistance)) {
						listScroller.y = -(listScroller.height -scrollDistance);
				
					}
					
				} else {
					listScroller.y = 0;
				}

			}
		}
			
			
	}
}