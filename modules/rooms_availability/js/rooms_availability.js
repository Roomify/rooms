(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
    
    $('#calendar').fullCalendar({
      editable:true,
      month:5,
      events: Drupal.settings.basePath + 'admin/rooms/rooms/room/ajax/test'
    });
    

    $('#calendar1').fullCalendar({
      editable:true,
      month:6,
      events: Drupal.settings.basePath + 'admin/rooms/rooms/room/ajax/test'
    });

    $('#calendar2').fullCalendar({
      editable:true,
      month:7,
      events: Drupal.settings.basePath + 'admin/rooms/rooms/room/ajax/test'
    });


  }
};
})(jQuery);
  
  

