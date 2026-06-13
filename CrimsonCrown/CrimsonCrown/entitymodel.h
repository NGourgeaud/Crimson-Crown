// File Name: entitymodel.h
// Group: A11
// Description: Header file defining the base abstract class for all physical entities and movement directions.

#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H
#include <QRect>
#include <QPointF>

///
/// \brief Enumeration defining all possible 8-way movement directions for entities.
///
enum Direction{
    Left,
    Right,
    Up,
    Down,
    LeftDown,
    LeftUp,
    RightDown,
    RightUp
};

class EntityModel
{
protected:
    int itsHP;
    int itsMaxHP;
    int itsSpeed;
    QRectF itsHitBox;

public:
    ///
    /// \brief Default constructor.
    ///
    EntityModel();

    ///
    /// \brief Default virtual destructor.
    ///
    virtual ~EntityModel() = default;

    ///
    /// \brief Pure virtual function to handle health point deduction.
    /// \param amount The amount of damage taken.
    ///
    virtual void takeDamage(int amount) = 0;

    ///
    /// \brief Pure virtual function to force the entity to a specific coordinate.
    /// \param pos The target position.
    ///
    virtual void move(QPointF pos) = 0;

    ///
    /// \brief Pure virtual function to retrieve the entity's geometric bounding box.
    /// \return A QRectF representing the physical space.
    ///
    virtual QRectF getHitBox() const = 0;
};

#endif // ENTITYMODEL_H
