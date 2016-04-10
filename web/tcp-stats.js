$(function() {
	$(".analysis").each(function() {
		var body = $(this).children(".analysis-body");
		var arrow = $(this).find(".analysis-header-arrow");

		$(body).hide();

		$(this).click(function() {
			if ($(body).is(":visible")) {
				$(body).slideUp();
				$(arrow).removeClass("rotate-down");
				$(arrow).addClass("rotate-up");
			}
			else {
				$(body).slideDown();
				$(arrow).removeClass("rotate-up");
				$(arrow).addClass("rotate-down");
			}
		});
	});
});
