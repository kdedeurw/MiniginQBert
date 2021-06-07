#pragma once

//disable "prefer enum class over enum" warning to allow implicit typecasting
#pragma warning(disable:26812)

enum QBertEvent
{
	event_player_die,
	event_tile_colour_change_intermediate,
	event_tile_colour_change_final,
	event_green_ball_catched,
	event_coily_killed,
	event_slick_catched,
	event_sam_catched,
	event_round_complete,
	event_level_complete,
	event_spinning_pad_used,
};