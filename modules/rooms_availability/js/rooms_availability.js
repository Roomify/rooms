(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
        
    $currentmonth = parseInt(Drupal.settings.currentMonth);
    if ($currentmonth == 12) {
      $nextmonth = 1;
    }
    if ($currentmonth == 1) {
      $pastmonth == 12;
    }
    else{
      $nextmonth = parseInt(Drupal.settings.currentMonth)+1;
      $pastmonth = parseInt(Drupal.settings.currentMonth)-1;
    }
    
    $('#calendar').fullCalendar({
      editable:false,
      month:$pastmonth-1,
      header:{
        left: 'title',
        center: '',
        right: ''
      },
      events: Drupal.settings.basePath + 'admin/rooms/rooms/room/' + Drupal.settings.roomID + '/availability/json/' + Drupal.settings.currentYear + '/' + $pastmonth
    });
    
    $('#calendar1').fullCalendar({
      editable:false,
      header:{
        left: 'title',
        center: '',
        right: ''
      },
      month:$currentmonth-1,
      events: Drupal.settings.basePath + 'admin/rooms/rooms/room/' + Drupal.settings.roomID + '/availability/json/' + Drupal.settings.currentYear + '/' + $currentmonth
    });

    $('#calendar2').fullCalendar({
      editable:false,
      month:$nextmonth-1,
      header:{
        left: 'title',
        center: '',
        right: ''
      },
      events: Drupal.settings.basePath + 'admin/rooms/rooms/room/' + Drupal.settings.roomID + '/availability/json/' + Drupal.settings.currentYear + '/' + $nextmonth
    });

    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);
  
  

