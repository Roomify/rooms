(function ($) {

Drupal.behaviors.rooms_date_popup = {
  attach: function(context) {
    var startYear = Drupal.settings.rooms.roomsStartYear;
    var startMonth = Drupal.settings.rooms.roomsStartMonth;
    var dates = $( "#edit-rooms-start-date-datepicker-popup-0, #edit-rooms-end-date-datepicker-popup-0" ).datepicker({
      dateFormat: Drupal.settings.rooms.roomsDateFormat,
      minDate: new Date(startYear, startMonth - 1, 01),
      maxDate: new Date(startYear, startMonth - 1,
        32 - new Date(startYear, startMonth - 1, 32).getDate()),
      defaultDate: new Date(startYear, startMonth - 1, 01),
      numberOfMonths: 1,
      onSelect: function( selectedDate ) {
        var option = this.id == "edit-rooms-start-date-datepicker-popup-0" ? "minDate" : "na", //can change na to maxDate to set the range
          instance = $( this ).data( "datepicker" ),
          date = $.datepicker.parseDate(
            instance.settings.dateFormat ||
            $.datepicker._defaults.dateFormat,
            selectedDate, instance.settings );
        dates.not( this ).datepicker( "option", option, date );
      },
      beforeShow: function()
      {
        setTimeout(function()
          {
            // If you think this is ugly you are right - read this though: http://blog.foersom.dk/post/598839422/dealing-with-z-index-in-jquery-uis-datepicker
            $(".ui-datepicker").css("z-index", 12);
          }, 10);
      }
    });
  }
};
})(jQuery);
