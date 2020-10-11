#pragma once

#include <pera_software/aidkit/qt/properties/Property.hpp>
#include <chrono>

class QSpinBox;

class MillisecondsProperty : public pera_software::aidkit::qt::Property<std::chrono::milliseconds> {
	Q_OBJECT
	public:
		using Property<std::chrono::milliseconds>::Property;
		using Property<std::chrono::milliseconds>::operator=;

	Q_SIGNALS:
		void valueChanged(std::chrono::milliseconds newValue) override;

	protected:
		void emitValueChanged(std::chrono::milliseconds value) override;
};

void bindSpinBoxValueProperty(QSpinBox *spinBox, MillisecondsProperty *property);
