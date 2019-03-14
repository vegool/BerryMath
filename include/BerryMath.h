/*****************************************************************************
*  BerryMath Interpreter                                                     *
*  Copyright (C) 2019 BerryMathDevelopmentTeam  zhengyh2018@gmail.com        *
*                                                                            *
*  这个文件是整合所有文件的，制作拓展或者嵌入BerryMath都需要include该文件.           *
*                                                                            *
*  This program is free software; you can redistribute it and/or modify      *
*  it under the terms of the GNU General Public License version 3 as         *
*  published by the Free Software Foundation.                                *
*                                                                            *
*  You should have received a copy of the GNU General Public License         *
*  along with OST. If not, see <http://www.gnu.org/licenses/>.               *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*  @file     BerryMath.h                                                     *
*  @brief    接口文件                                                         *
*  Details.                                                                  *
*                                                                            *
*  @author   yhzheng                                                         *
*  @email    zhengyh2018@gmail.com                                           *
*  @version  0.0.1 (beta)                                                    *
*  @date     2019.2.19.                                                      *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*****************************************************************************/

#ifndef BERRYMATH_BERRYMATH_H
#define BERRYMATH_BERRYMATH_H

#ifndef __cplusplus
#error ProgrammingError: The BerryMath language is written in C++. Please compile BerryMath using the C++ compiler (g++).
#endif

#define DEBUG

#include <iostream>
#include <fstream>
#include <json.h>
#include "btype.h"
#include "AST.h"
#include "lex.h"
#include "color.h"
#include "memory.h"
#include "script.h"

/**
 * @brief 所有的BerryMath接口都在BerryMath内
 */
namespace BerryMath {
}

#endif //BERRYMATH_BERRYMATH_H