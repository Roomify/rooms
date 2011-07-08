(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
    
    //alert((1) + parseInt(Drupal.settings.currentMonth));
    //alert(Drupal.settings.basePath + 'admin/rooms/rooms/room/' + Drupal.settings.roomID + '/availability/json/' + Drupal.settings.currentYear + '/' + parseInt(Drupal.settings.currentMonth)-1);
    $pastmonth = parseInt(Drupal.settings.currentMonth)-1;
    $currentmonth = parseInt(Drupal.settings.currentMonth);
    $nextmonth = parseInt(Drupal.settings.currentMonth)+1;
    
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


  }
};
})(jQuery);
  
  

