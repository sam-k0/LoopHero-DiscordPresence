#pragma once
#include <string>
#include "PluginSetup.h"
#include "LH/LHObjects.h"

// Useful bindings to UI related functionality
namespace LHUI 
{

	class UIElementBase
	{
	protected:
		double ref;
	public: 
		void SetDepth(double depth)
		{
			Binds::CallBuiltinA("variable_instance_set", { ref, "depth", depth});
		}

		bool Exists()
		{
			return (bool)Binds::CallBuiltinA("instance_exists", { ref }).As<int>();
		}

		double GetRef()
		{
			return ref;
		}
	};

	class Button : private UIElementBase{
	public:
		Button(const char* buttonText, double x, double y, double depth = -200.0)
		{
			double buttonRef = static_cast<double>(Binds::CallBuiltinA("instance_create_depth", { x, y, depth, (double)LHObjectEnum::o_continue_button }));
			Binds::CallBuiltinA("variable_instance_set", { buttonRef, "click_event", -1. }); // delete original callback
			Binds::CallBuiltinA("variable_instance_set", { buttonRef, "text", buttonText });
			Binds::CallBuiltinA("variable_instance_set", { buttonRef, "fa_ltext", buttonText });
			this->ref = buttonRef;
		}

		void SetText(const char* buttonText)
		{
			Binds::CallBuiltinA("variable_instance_set", { this->ref, "text", buttonText });
			Binds::CallBuiltinA("variable_instance_set", { this->ref, "fa_ltext", buttonText });
		}

	};

	class Message : private UIElementBase
	{
	public:
		// Creates a message in the foreground that takes focus
		double CreateMessage(const char* textMessage, const char* textButton, double x, double y, double depth = -200.0)
		{
			double msgRef = static_cast<double>(Binds::CallBuiltinA("instance_create_depth", { x, y, depth, (double)LHObjectEnum::o_menu_message }));
			Binds::CallBuiltinA("variable_instance_set", { msgRef, "text_message", textMessage });
			Binds::CallBuiltinA("variable_instance_set", { msgRef, "text", textButton });
			this->ref = msgRef;
		}

		void SetMessageText(const char* textMessage)
		{
			Binds::CallBuiltinA("variable_instance_set", { this->ref, "text_message", textMessage });
		}

		void SetButtonText(const char* textButton)
		{
			Binds::CallBuiltinA("variable_instance_set", { this->ref, "text", textButton });
		}
	};

	class Slider : private UIElementBase
	{
		// Creates a slider, if steps is > 0, automatically makes only x states possible
		Slider(double x, double y, double width, double depth = -200.0, int steps = -1)
		{
			double sliderRef = static_cast<double>(Binds::CallBuiltinA("instance_create_depth", { x, y, depth, (double)LHObjectEnum::o_slider }));
			Binds::CallBuiltinA("variable_instance_set", { sliderRef, "width", width });
			Binds::CallBuiltinA("variable_instance_set", { sliderRef, "steps", (double)steps });
			this->ref = sliderRef;
		}

		void SetSteps(int steps = -1)
		{
			Binds::CallBuiltinA("variable_instance_set", { this->ref, "steps", (double)steps });
		}

		void SetWidth(double width)
		{
			Binds::CallBuiltinA("variable_instance_set", { this->ref, "width", width });
		}

		double GetValue()
		{
			return Binds::CallBuiltinA("variable_instance_get", { this->ref, "trueval"}).As<double>();
		}
	};
}