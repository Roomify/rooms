(function ($) {

/*
 * Default settings for all Rooms datepickers; they come in pairs.
 */
Drupal.settings.rooms_datepicker = {
  // When the startDate is selected, update the minDate attribute of the
  // endDate picker so users can't pick an inverse date range.
  onSelect: function(selectedDate) {
    instance = $(this).data("datepicker");
    format = instance.settings.dateFormat || $.datepicker._defaults.dateFormat;
    date = $.datepicker.parseDate(format, selectedDate, instance.settings);
    if (instance.settings.endDate !== undefined) {
      instance.settings.endDate.datepicker("option", "minDate", date);
    }
  },

  // If you think this is ugly you are right - read this though:
  // http://blog.foersom.dk/post/598839422/dealing-with-z-index-in-jquery-uis-datepicker
  beforeShow: function() {
    setTimeout(function() {
      $(".ui-datepicker").css("z-index", 12);
    }, 10);
  },
};

/*
 * Defines the Rooms Date Popup behavior for all start and end date range pairs.
 */
Drupal.behaviors.rooms_datepicker = {
  attach: function(context) {
    // Iterate through pairs of date fields (there may be multiple ones on
    // a page) and setup the jQuery datepickers combining default settings
    // with any custom settings provided. Date ranges pairs are defined by
    // the Drupal.settings.rooms.datepickers object, keyed by the unique id
    // of the startDate wrapper element.
    for (var startDateId in Drupal.settings.rooms.datepickers) {
      var endDateId = Drupal.settings.rooms.datepickers[startDateId].endDateId;
      var endDate = $("#" + endDateId + ' .form-text').datepicker(
        $.extend(
          Drupal.settings.rooms_datepicker, // Defaults defined above.
          Drupal.settings.rooms.datepickers[startDateId].settings // Settings from Drupal.
        )
      );
      var startDate = $("#" + startDateId+ ' .form-text').datepicker(
        $.extend(
          { 'endDate': endDate }, // Reference to endDate picker.
          Drupal.settings.rooms_datepicker, // Defaults.
          Drupal.settings.rooms.datepickers[startDateId].settings // Settings from Drupal.
        )
      );
    }
  }
};
})(jQuery);
