#ifndef DOWNLOADEXTRACTWRAPPER_H
#define DOWNLOADEXTRACTWRAPPER_H

/***************************************************************************
 * This file is part of the TTK Downloader project
 * Copyright (C) 2015 - 2022 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkglobaldefine.h"

struct DownloadBackgroundImage;

/*! @brief The class of the extract data wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT DownloadExtractWrapper
{
    TTK_DECLARE_MODULE(DownloadExtractWrapper)
public:
    /*!
     * Transfer file to image data.
     */
    static bool outputThunderSkin(QPixmap &image, const QString &path);

    /*!
     * Transfer file to image data.
     */
    static bool outputSkin(DownloadBackgroundImage *image, const QString &path);
    /*!
     * Transfer image data to file.
     */
    static bool inputSkin(DownloadBackgroundImage *image, const QString &path);

    /*!
     * Transfer file to data.
     */
    static bool outputText(QByteArray &data, const QString &path);
    /*!
     * Transfer data to file.
     */
    static bool inputText(const QByteArray &data, const QString &path);

};

#endif // DOWNLOADEXTRACTWRAPPER_H
