(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
        
    $currentmonth = parseInt(Drupal.settings.currentMonth);
    $currentyear = parseInt(Drupal.settings.currentYear);
    if ($currentmonth == 12) {
      $nextmonth1 = 1;
      $nextyear1 = parseInt(Drupal.settings.currentYear) + 1;
      $nextmonth2 = 2;
      $nextyear2 = parseInt(Drupal.settings.currentYear) + 1;
    }
    if ($currentmonth == 11){
      $nextmonth1 = 12;
      $nextyear1 = Drupal.settings.currentYear;
      $nextmonth2 = 2;
      $nextyear2 = parseInt(Drupal.settings.currentYear) + 1;
    }
    else{
      $nextmonth1 = $currentmonth+1;
      $nextyear1 = $currentyear;
      $nextmonth2 = $currentmonth+2;
      $nextyear2 = $currentyear;
    }
    
    $('#calendar').fullCalendar({
      editable:false,
      month:$currentmonth-1,
      year:Drupal.settings.currentYear,
      header:{
        left: 'title',
        center: '',
        right: ''
      },
      events: Drupal.settings.basePath + 'admin/rooms/units/unit/' + Drupal.settings.roomID + '/availability/json/' + $currentyear + '/' + $currentmonth
    });
    
    $('#calendar1').fullCalendar({
      editable:false,
      month:$nextmonth1-1,
      year:Drupal.settings.currentYear,
      header:{
        left: 'title',
        center: '',
        right: ''
      },
      events: Drupal.settings.basePath + 'admin/rooms/units/unit/' + Drupal.settings.roomID + '/availability/json/' + $nextyear1 + '/' + $nextmonth1
    });

    $('#calendar2').fullCalendar({
      editable:false,
      month:$nextmonth2-1,
      year:Drupal.settings.currentYear,
      header:{
        left: 'title',
        center: '',
        right: ''
      },
      events: Drupal.settings.basePath + 'admin/rooms/units/unit/' + Drupal.settings.roomID + '/availability/json/' + $nextyear2 + '/' + $nextmonth2,
      eventClick: function(calEvent, jsEvent, view) {
        
        alert('Event: ' + calEvent.title);
        location.reload();
        
      }
 
    });

  
    // Resize takes care of some quirks on occasion
    $(window).resize();
    
    

  }
};
})(jQuery);
  
  

