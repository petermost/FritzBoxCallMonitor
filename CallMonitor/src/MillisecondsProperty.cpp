#include "MillisecondsProperty.hpp"
#include <QSpinBox>

using namespace std::chrono;

void MillisecondsProperty::emitValueChanged(std::chrono::milliseconds value)
{
	Q_EMIT valueChanged(value);
}

void bindSpinBoxValueProperty(QSpinBox *spinBox, MillisecondsProperty *property)
{
	spinBox->setValue(static_cast<int>(property->value().count()));

	QSpinBox::connect(spinBox, qOverload<int>(&QSpinBox::valueChanged), [=](int value) {
		property->setValue(milliseconds(value));
	});

	MillisecondsProperty::connect(property, &MillisecondsProperty::valueChanged, [=](milliseconds timeout) {
		spinBox->setValue(static_cast<int>(timeout.count()));
	});
}
