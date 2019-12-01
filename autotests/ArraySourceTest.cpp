/*
 * This file is part of KQuickCharts
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <https://www.gnu.org/licenses/>.
 */

#include <QTest>

#include "datasource/ArraySource.h"

class ArraySourceTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testCreate()
    {
        // Basic creation should create an empty source.
        auto source = new ArraySource{};

        QCOMPARE(source->itemCount(), 0);
        QCOMPARE(source->item(0), QVariant{});
        QCOMPARE(source->minimum(), QVariant{});
        QCOMPARE(source->maximum(), QVariant{});
    }

    void testWithArray_data()
    {
        QTest::addColumn<QVariantList>("array");
        QTest::addColumn<int>("itemCount");
        QTest::addColumn<QVariant>("firstItem");
        QTest::addColumn<QVariant>("lastItem");
        QTest::addColumn<QVariant>("minimum");
        QTest::addColumn<QVariant>("maximum");

        QTest::newRow("simple ints") << QVariantList{0, 1, 2, 3, 4} << 5 << QVariant{0} << QVariant{4} << QVariant{0} << QVariant{4};
        QTest::newRow("random ints") << QVariantList{-3, 6, 4, 9, 4} << 5 << QVariant{-3} << QVariant{4} << QVariant{-3} << QVariant{9};
        QTest::newRow("floats") << QVariantList{2.56, 4.45, 2.5, 5.3} << 4 << QVariant{2.56} << QVariant{5.3} << QVariant{2.5} << QVariant{5.3};
    }

    void testWithArray()
    {
        auto source = new ArraySource{};
        QFETCH(QVariantList, array);
        source->setArray(array);

        QFETCH(int, itemCount);
        QCOMPARE(source->itemCount(), itemCount);

        QFETCH(QVariant, firstItem);
        QCOMPARE(source->item(0), firstItem);

        QFETCH(QVariant, lastItem);
        QCOMPARE(source->item(itemCount - 1), lastItem);

        QCOMPARE(source->item(itemCount), QVariant{});

        QFETCH(QVariant, minimum);
        QCOMPARE(source->minimum(), minimum);

        QFETCH(QVariant, maximum);
        QCOMPARE(source->maximum(), maximum);
    }

    void testWrap_data()
    {
        QTest::addColumn<QVariantList>("array");
        QTest::addColumn<int>("itemCount");
        QTest::addColumn<QVariant>("firstItem");
        QTest::addColumn<QVariant>("lastItem");

        QTest::newRow("simple ints") << QVariantList{0, 1, 2, 3, 4} << 5 << QVariant{0} << QVariant{4};
        QTest::newRow("random ints") << QVariantList{-3, 6, 4, 9, 4} << 5 << QVariant{-3} << QVariant{4};
        QTest::newRow("floats") << QVariantList{2.56, 4.45, 2.5, 5.3} << 4 << QVariant{2.56} << QVariant{5.3};
    }

    void testWrap()
    {
        auto source = new ArraySource{};
        QFETCH(QVariantList, array);
        source->setArray(array);
        source->setWrap(true);

        QFETCH(int, itemCount);
        QFETCH(QVariant, firstItem);
        QFETCH(QVariant, lastItem);

        QCOMPARE(source->item(0), firstItem);
        QCOMPARE(source->item(itemCount - 1), lastItem);
        QCOMPARE(source->item(itemCount), firstItem);
        QCOMPARE(source->item(itemCount * 2 - 1), lastItem);

        QCOMPARE(source->item(itemCount * 99), firstItem);
    }
};


QTEST_MAIN(ArraySourceTest)

#include "ArraySourceTest.moc"
