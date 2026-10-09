/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
package rem.compiler;

import java.util.*;

final class DetType {

  private byte internalType=0;
  static final protected byte det=1;
  static final protected byte semiDet=2;
  static final protected byte multiDet=3;
  static final protected byte nonDet=4;

  static public DetType detType = new DetType(det);
  static public DetType semiDetType = new DetType(semiDet);
  static public DetType multiDetType = new DetType(multiDet);
  static public DetType nonDetType = new DetType(nonDet);

  public DetType(byte type) {
    this.internalType=type;
  }

  private byte getDetType() {
    return internalType;
  }

  public boolean isExactlyDet() {
    return getDetType()==det;
  }
  public boolean isExactlySemiDet() {
    return getDetType()==semiDet;
  }
  public boolean isExactlyMultiDet() {
    return getDetType()==multiDet;
  }
  public boolean isExactlyNonDet() {
    return getDetType()==nonDet;
  }

  public boolean isDet() {
    return isExactlyDet();
  }
  public boolean isSemiDet() {
    return isDet() || isExactlySemiDet();
  }
  public boolean isMultiDet() {
    return isDet() || isExactlyMultiDet();
  }
  public boolean isNonDet() {
    return isExactlyNonDet() || isSemiDet() || isMultiDet();
  }

  public DetType and(DetType type) {
    if(isDet()) {
      return type;
    } else if(isSemiDet()) {
      if(type.isDet() || type.isSemiDet()) {
	return semiDetType;
      } else {
	return nonDetType;
      }
    } else if(isMultiDet()) {
      if(type.isDet() || type.isMultiDet()) {
	return multiDetType;
      } else {
	return nonDetType;
      }
    } else if(isNonDet()) {
      return nonDetType;
    } else {
      throw new InternalError("unknown DetType");
    }
  }

  public DetType or(DetType type) {
    if(isDet()) {
      return multiDetType;
    } else if(isSemiDet()) {
      if(type.isDet() || type.isMultiDet()) {
	return multiDetType;
      } else {
	return nonDetType;
      }
    } else if(isMultiDet()) {
      return multiDetType;
    } else if(isNonDet()) {
      if(type.isDet() || type.isMultiDet()) {
	return multiDetType;
      } else {
	return nonDetType;
      }
    } else {
      throw new InternalError("unknown DetType");
    }
  }

  public String toString() {
    if(isExactlyDet()) {
      return "det";
    } else if(isExactlySemiDet()) {
      return "semiDet";
    } else if(isExactlyMultiDet()) {
      return "multiDet";
    } else if(isExactlyNonDet()) {
      return "nonDet";
    } else {
      throw new InternalError("unknown DetType");
    }
  }
}

