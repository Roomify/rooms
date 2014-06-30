(function ($) {

/**
 * Similar to sticky-enabled tables from core Drupal, this allows the legend
 * on the far right side of the booking form to stay in place as the user
 * scrolls down the page to view the options.
 */
Drupal.behaviors.rooms_sticky_booking_legend = {
  attach: function(context) {

    if ($('.booking-legend').offset() !== null) {
      var top = $('.booking-legend').offset().top - parseFloat($('.booking-legend').css('marginTop').replace(/auto/, 0));
      $(window).scroll(function (event) {
        // How far has the user scrolled vertically.
        var y = $(this).scrollTop();

        // When the user scrolls past the top of the booking legend.
        if (y >= top) {
          // Fix the legend in place on the page.
          $('.booking-legend').addClass('is-fixed');
        } else {
          // Otherwise un-fix it.
          $('.booking-legend').removeClass('is-fixed');
        }
      });
    }
  }
};

})(jQuery);
