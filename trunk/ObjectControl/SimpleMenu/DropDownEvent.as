﻿package {
	
	import flash.events.Event;
	
		public class DropDownEvent extends Event{
			
			public var selectedId:int;
			
			public var selectedLabel:String;
			
			public var selectedData:*;
			
			public function DropDownEvent(type:String, _selectedId:int, _selectedLabel:String, _selectedData:*, bubbles:Boolean = false, cancelable:Boolean = false){
				
				super(type, bubbles,cancelable);
				
				selectedId = _selectedId;
				
				selectedLabel = _selectedLabel;
				
				selectedData = _selectedData;
				
		}
	}
	
}