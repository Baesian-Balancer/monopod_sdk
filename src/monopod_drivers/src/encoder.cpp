/**
 * @file motor.cpp
 * @author Manuel Wuthrich (manuel.wuthrich@gmail.com)
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief
 * @version 0.1
 * @date 2018-11-27
 *
 * @copyright Copyright (c) 2018
 *
 */

#include <monopod_sdk/blmc_drivers/devices/motor.hpp>

namespace blmc_drivers
{
Encoder::Encoder(Ptr<MotorBoardInterface> board, bool encoder_id)
    : board_(board), encoder_id_(encoder_id)
{
}

Encoder::Ptr<const Motor::ScalarTimeseries> Motor::get_measurement(
    const int& index) const
{
    if (encoder_id_ == 0)
    {
        switch (index)
        {
            case current:
                return board_->get_measurement(MotorBoardInterface::current_0);
            case position:
                return board_->get_measurement(MotorBoardInterface::position_0);
            case velocity:
                return board_->get_measurement(MotorBoardInterface::velocity_0);
            case encoder_index:
                return board_->get_measurement(
                    MotorBoardInterface::encoder_index_0);
        }
    }
    else
    {
        switch (index)
        {
            case current:
                return board_->get_measurement(MotorBoardInterface::current_1);
            case position:
                return board_->get_measurement(MotorBoardInterface::position_1);
            case velocity:
                return board_->get_measurement(MotorBoardInterface::velocity_1);
            case encoder_index:
                return board_->get_measurement(
                    MotorBoardInterface::encoder_index_1);
        }
    }

    throw std::invalid_argument("index needs to match one of the measurements");
}


void Encoder::print() const
{
    MotorBoardStatus motor_board_status;
    double motor_current = std::nan("");
    double motor_position = std::nan("");
    double motor_velocity = std::nan("");
    double motor_encoder_index = std::nan("");
    double motor_sent_current_target = std::nan("");

    if (board_->get_status()->length() != 0)
    {
        motor_board_status = board_->get_status()->newest_element();
    }

    if (get_measurement(current)->length() != 0)
    {
        motor_current = get_measurement(current)->newest_element();
    }

    if (get_measurement(position)->length() != 0)
    {
        motor_position = get_measurement(position)->newest_element();
    }

    if (get_measurement(velocity)->length() != 0)
    {
        motor_velocity = get_measurement(velocity)->newest_element();
    }

    if (get_measurement(encoder_index)->length() != 0)
    {
        motor_encoder_index = get_measurement(encoder_index)->newest_element();
    }

    if (get_sent_current_target()->length() != 0)
    {
        motor_sent_current_target = get_sent_current_target()->newest_element();
    }

    rt_printf("motor board status: ");
    rt_printf("enabled: %d ", motor_board_status.system_enabled);
    rt_printf("error_code: %d ", motor_board_status.error_code);
    rt_printf("motor status: ");
    if (motor_id_ == 0)
    {
        rt_printf("enabled: %d ", motor_board_status.motor1_enabled);
        rt_printf("ready: %d ", motor_board_status.motor1_ready);
    }
    else
    {
        rt_printf("enabled: %d ", motor_board_status.motor2_enabled);
        rt_printf("ready: %d ", motor_board_status.motor2_ready);
    }
    rt_printf("motor measurements: ");
    rt_printf("current: %8f ", motor_current);
    rt_printf("position: %8f ", motor_position);
    rt_printf("velocity: %8f ", motor_velocity);
    rt_printf("encoder index: %8f ", motor_encoder_index);
    rt_printf("target current: %8f ", motor_sent_current_target);
    rt_printf("\n");
}

}  // namespace blmc_drivers
