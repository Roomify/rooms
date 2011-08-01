(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
        
    currentmonth = parseInt(Drupal.settings.roomsAvailability.currentMonth);
    currentyear = parseInt(Drupal.settings.roomsAvailability.currentYear);
    if (currentmonth == 12) {
      nextmonth1 = 1;
      nextyear1 = parseInt(Drupal.settings.roomsAvailability.currentYear) + 1;
      nextmonth2 = 2;
      nextyear2 = parseInt(Drupal.settings.roomsAvailability.currentYear) + 1;
    }
    if (currentmonth == 11){
      nextmonth1 = 12;
      nextyear1 = Drupal.settings.roomsAvailability.currentYear;
      nextmonth2 = 2;
      nextyear2 = parseInt(Drupal.settings.roomsAvailability.currentYear) + 1;
    }
    else{
      nextmonth1 = currentmonth+1;
      nextyear1 = currentyear;
      nextmonth2 = currentmonth+2;
      nextyear2 = currentyear;
    }
    
    var calendars = new Array();
    calendars[0] = new Array('#calendar', currentmonth, currentyear);
    calendars[1] = new Array('#calendar1', nextmonth1, nextyear1);
    calendars[2] = new Array('#calendar2', nextmonth2, nextyear2);
    
    
    $.each(calendars, function(key, value) {
      $(value[0]).once().fullCalendar({
        ignoreTimezone:false,
        editable:false,
        month:value[1]-1,
        year:Drupal.settings.roomsAvailability.currentYear,
        header:{
          left: 'title',
          center: '',
          right: ''
        },
        events: Drupal.settings.basePath + '?q=admin/rooms/units/unit/' + Drupal.settings.roomsAvailability.roomID + '/availability/json/' + value[2] + '/' + value[1],
        eventClick: function(calEvent, jsEvent, view) {
          // Getting the Unix timestamp - JS will only give us milliseconds
          if (calEvent.end == null) {
            //We are probably dealing with a single day event
            calEvent.end = calEvent.start;
          }
          date = $.fullCalendar.parseDate(calEvent.start)
          var sd = Math.round(Date.parse(calEvent.start)/1000);
          var ed = Math.round(Date.parse(calEvent.end)/1000);
          if ($.colorbox) {
            
            var url = Drupal.settings.basePath + '?q=admin/rooms/units/unit/' + Drupal.settings.roomsAvailability.roomID + '/event/' + calEvent.id + '/' + sd + '/' + ed; 
            $.colorbox({
              href: url,
              opacity:0.7,
              width: 400,
              height: 400,
              onClosed:function(){
                $(value[0]).fullCalendar('refetchEvents');  
                //$('#calendar').fullCalendar('rerender');
              }
            });   
          }
        }
//        eventMouseover : function(event, jsEvent, view) {
//          $(value[0]).css('border', '10px solid red');
//          //event.color = 'yellow';
//          $(value[0]).fullCalendar('updateEvent', event);
//        },
//        eventMouseout : function(event, jsEvent, view) {
//          //$(this).css('border', '10px solid red');
//          event.title = 'red';
//          $(value[0]).fullCalendar('updateEvent', event);
//        }
      });
    });
    
    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);
  
  

