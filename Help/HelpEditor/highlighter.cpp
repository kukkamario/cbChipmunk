#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bAbs\\b"<< "\\bACos\\b"<< "\\bAddText\\b"<< "\\bAfter\\b"<< "\\bAnd\\b"<< "\\bAnimationHeight\\b"<< "\\bAnimationPlaying\\b"<< "\\bAnimationWidth\\b"<< "\\bAs\\b"<< "\\bAsc\\b"<< "\\bASin\\b"<< "\\bATan\\b"<< "\\bBefore\\b"<< "\\bBin\\b"<< "\\bBLUE\\b"<< "\\bBox\\b"<< "\\bBoxOverlap\\b"<< "\\bByte\\b"<< "\\bCallDLL\\b"<< "\\bCameraAngle\\b"<< "\\bCameraFollow\\b"<< "\\bCameraPick\\b"<< "\\bCameraX\\b"<< "\\bCameraY\\b"<< "\\bCase\\b"<< "\\bCenterText\\b"<< "\\bChDir\\b"<< "\\bChr\\b"<< "\\bCircle\\b"<< "\\bClearArray\\b"<< "\\bClearCollisions\\b"<< "\\bClearKeys\\b"<< "\\bClearMouse\\b"<< "\\bClearObjects\\b"<< "\\bClearText\\b"<< "\\bCloneCameraOrientation\\b"<< "\\bCloneCameraPosition\\b"<< "\\bCloneImage\\b"<< "\\bCloneObject\\b"<< "\\bCloneObjectOrientation\\b"<< "\\bCloneObjectPosition\\b"<< "\\bCloseFile\\b"<< "\\bCloseInput\\b"<< "\\bCls\\b"<< "\\bClsColor\\b"<< "\\bCollisionAngle\\b"<< "\\bCollisionX\\b"<< "\\bCollisionX\\b"<< "\\bColor\\b"<< "\\bCommandLine\\b"<< "\\bConst\\b"<< "\\bConvertToInteger\\b"<< "\\bConvertToType\\b"<< "\\bCopyBox\\b"<< "\\bCopyFile\\b"<< "\\bCos\\b"<< "\\bCountCollisions\\b"<< "\\bCountWords\\b"<< "\\bCrc32\\b"<< "\\bCurrentDir\\b"<< "\\bCurveAngle\\b"<< "\\bCurveValue\\b"<< "\\bData\\b"<< "\\bDate\\b"<< "\\bDecrypt\\b"<< "\\bDefault\\b"<< "\\bDefaultMask\\b"<< "\\bDefaultVisible\\b"<< "\\bDelete\\b"<< "\\bDeleteFile\\b"<< "\\bDeleteFont\\b"<< "\\bDeleteImage\\b"<< "\\bDeleteMEMBlock\\b"<< "\\bDeleteObject\\b"<< "\\bDeleteSound\\b"<< "\\bDim\\b"<< "\\bDistance\\b"<< "\\bDistance2\\b"<< "\\bDot\\b"<< "\\bDownKey\\b"<< "\\bDrawAnimation\\b"<< "\\bDrawGame\\b"<< "\\bDrawGhostImage\\b"<< "\\bDrawImage\\b"<< "\\bDrawImageBox\\b"<< "\\bDrawScreen\\b"<< "\\bDrawToImage\\b"<< "\\bDrawToScreen\\b"<< "\\bDrawToWorld\\b"<< "\\bEach\\b"<< "\\bEditMap\\b"<< "\\bEllipse\\b"<< "\\bElse\\b"<< "\\bElseIf\\b"<< "\\bEncrypt\\b"<< "\\bEnd\\b"<< "\\bEndFunction\\b"<< "\\bEndIf\\b"<< "\\bEndSelect\\b"<< "\\bEndType\\b"<< "\\bEndSearch\\b"<< "\\bEOF\\b"<< "\\bErrors\\b"<< "\\bEscapeKey\\b"<< "\\bExecute\\b"<< "\\bExit\\b"<< "\\bFalse\\b"<< "\\bField\\b"<< "\\bFileExists\\b"<< "\\bFileOffset\\b"<< "\\bFileSize\\b"<< "\\bFindFile\\b"<< "\\bFirst\\b"<< "\\bFlip\\b"<< "\\bFloat\\b"<< "\\bFor\\b"<< "\\bForever\\b"<< "\\bFPS\\b"<< "\\bFrameLimit\\b"<< "\\bFunction\\b"<< "\\bGetAngle\\b"<< "\\bGetAngle2\\b"<< "\\bGetCollision\\b"<< "\\bGetEXEName\\b"<< "\\bGetKey\\b"<< "\\bGetMap\\b"<< "\\bGetMap2\\b"<< "\\bGetMouse\\b"<< "\\bGetPixel\\b"<< "\\bGetPixel2\\b"<< "\\bgetRGB\\b"<< "\\bGetWord\\b"<< "\\bGFXModeExists\\b"<< "\\bGhostObject\\b"<< "\\bGlobal\\b"<< "\\bGosub\\b"<< "\\bGoto\\b"<< "\\bGotoSavedLocation\\b"<< "\\bGREEN\\b"<< "\\bHex\\b"<< "\\bHotSpot\\b"<< "\\bIf\\b"<< "\\bImage\\b"<< "\\bImageHeight\\b"<< "\\bImagesCollide\\b"<< "\\bImagesOverlap\\b"<< "\\bImageWidth\\b"<< "\\bInclude\\b"<< "\\bInitObjectList\\b"<< "\\bInput\\b"<< "\\bInsert\\b"<< "\\bInStr\\b"<< "\\bInt\\b"<< "\\bIsDirectory\\b"<< "\\bKeyDown\\b"<< "\\bKeyHit\\b"<< "\\bKeyUp\\b"<< "\\bLast\\b"<< "\\bLeft\\b"<< "\\bLeftKey\\b"<< "\\bLen\\b"<< "\\bLine\\b"<< "\\bLoadAnimImage\\b"<< "\\bLoadAnimObject\\b"<< "\\bLoadFont\\b"<< "\\bLoadImage\\b"<< "\\bLoadMap\\b"<< "\\bLoadObject\\b"<< "\\bLoadProgram\\b"<< "\\bLoadSound\\b"<< "\\bLocate\\b"<< "\\bLock\\b"<< "\\bLog\\b"<< "\\bLog10\\b"<< "\\bLoopObject\\b"<< "\\bLower\\b"<< "\\bLSet\\b"<< "\\bMakeDir\\b"<< "\\bMakeEmitter\\b"<< "\\bMakeError\\b"<< "\\bMakeImage\\b"<< "\\bMakeMap\\b"<< "\\bMakeMEMBlock\\b"<< "\\bMakeObject\\b"<< "\\bMakeObjectFloor\\b"<< "\\bMapHeight\\b"<< "\\bMapWidth\\b"<< "\\bMaskImage\\b"<< "\\bMaskObject\\b"<< "\\bMax\\b"<< "\\bMEMBlockSize\\b"<< "\\bMemCopy\\b"<< "\\bMin\\b"<< "\\bMid\\b"<< "\\bMirrorObject\\b"<< "\\bMod\\b"<< "\\bMouseDown\\b"<< "\\bMouseHit\\b"<< "\\bMouseMoveX\\b"<< "\\bMouseMoveY\\b"<< "\\bMouseMoveZ\\b"<< "\\bMouseUp\\b"<< "\\bMouseWX\\b"<< "\\bMouseWY\\b"<< "\\bMouseX\\b"<< "\\bMouseY\\b"<< "\\bMouseX\\b"<< "\\bMoveCamera\\b"<< "\\bMoveObject\\b"<< "\\bNew\\b"<< "\\bNext\\b"<< "\\bNextObject\\b"<< "\\bNot\\b"<< "\\bNULL\\b"<< "\\bObjectAngle\\b"<< "\\bObjectFloat\\b"<< "\\bObjectFrame\\b"<< "\\bObjectInteger\\b"<< "\\bObjectLife\\b"<< "\\bObjectOrder\\b"<< "\\bObjectPick\\b"<< "\\bObjectPickable\\b"<< "\\bObjectPlaying\\b"<< "\\bObjectRange\\b"<< "\\bObjectSight\\b"<< "\\bObjectSizeX\\b"<< "\\bObjectSizeY\\b"<< "\\bObjectsOverlap\\b"<< "\\bObjectString\\b"<< "\\bObjectX\\b"<< "\\bObjectY\\b"<< "\\bOFF\\b"<< "\\bON\\b"<< "\\bOpenToEdit\\b"<< "\\bOpenToRead\\b"<< "\\bOpenToWrite\\b"<< "\\bOr\\b"<< "\\bPaintObject\\b"<< "\\bParticleAnimation\\b"<< "\\bParticleEmission\\b"<< "\\bParticleMovement\\b"<< "\\bPeekByte\\b"<< "\\bPeekFloat\\b"<< "\\bPeekInt\\b"<< "\\bPeekShort\\b"<< "\\bPI\\b"<< "\\bPickColor\\b"<< "\\bPickedAngle\\b"<< "\\bPickedObject\\b"<< "\\bPickedX\\b"<< "\\bPickedY\\b"<< "\\bPickImageColor\\b"<< "\\bPickImageColor2\\b"<< "\\bPixelPick\\b"<< "\\bPlayAnimation\\b"<< "\\bPlayObject\\b"<< "\\bPlaySound\\b"<< "\\bPointCamera\\b"<< "\\bPointObject\\b"<< "\\bPokeByte\\b"<< "\\bPokeFloat\\b"<< "\\bPokeInt\\b"<< "\\bPokeShort\\b"<< "\\bPositionCamera\\b"<< "\\bPositionMouse\\b"<< "\\bPositionObject\\b"<< "\\bPrint\\b"<< "\\bPutPixel\\b"<< "\\bPutPixel2\\b"<< "\\bRand\\b"<< "\\bRandomize\\b"<< "\\bRead\\b"<< "\\bReadByte\\b"<< "\\bReadFloat\\b"<< "\\bReadInt\\b"<< "\\bReadLine\\b"<< "\\bReadShort\\b"<< "\\bReadString\\b"<< "\\bRED\\b"<< "\\bReDim\\b"<< "\\bRepeat\\b"<< "\\bReplace\\b"<< "\\bResetObjectCollision\\b"<< "\\bResizeImage\\b"<< "\\bResizeMEMBlock\\b"<< "\\bRestore\\b"<< "\\bReturn\\b"<< "\\bRight\\b"<< "\\bRightKey\\b"<< "\\bRnd\\b"<< "\\bRotateCamera\\b"<< "\\bRotateImage\\b"<< "\\bRotateObject\\b"<< "\\bRoundDown\\b"<< "\\bRoundUp\\b"<< "\\bRSet\\b"<< "\\bSAFEEXIT\\b"<< "\\bSar\\b"<< "\\bSaveImage\\b"<< "\\bSaveProgram\\b"<< "\\bSCREEN\\b"<< "\\bScreenDepth\\b"<< "\\bScreenGamma\\b"<< "\\bScreenHeight\\b"<< "\\bScreenPositionObject\\b"<< "\\bScreenShot\\b"<< "\\bScreenWidth\\b"<< "\\bSeekFile\\b"<< "\\bSelect\\b"<< "\\bSetFont\\b"<< "\\bSetMap\\b"<< "\\bSetSound\\b"<< "\\bSetTile\\b"<< "\\bSetupCollision\\b"<< "\\bSetWindow\\b"<< "\\bShl\\b"<< "\\bShort\\b"<< "\\bShowMouse\\b"<< "\\bShowObject\\b"<< "\\bShr\\b"<< "\\bSin\\b"<< "\\bSmooth2D\\b"<< "\\bSoundPlaying\\b"<< "\\bSqrt\\b"<< "\\bStartSearch\\b"<< "\\bStep\\b"<< "\\bStopAnimation\\b"<< "\\bStopObject\\b"<< "\\bStopSound\\b"<< "\\bStr\\b"<< "\\bString\\b"<< "\\bStrInsert\\b"<< "\\bStrMove\\b"<< "\\bStrRemove\\b"<< "\\bTan\\b"<< "\\bText\\b"<< "\\bTextHeight\\b"<< "\\bTextWidth\\b"<< "\\bThen\\b"<< "\\bTime\\b"<< "\\bTimer\\b"<< "\\bTo\\b"<< "\\bTranslateCamera\\b"<< "\\bTranslateObject\\b"<< "\\bTrim\\b"<< "\\bTrue\\b"<< "\\bTurnCamera\\b"<< "\\bTurnObject\\b"<< "\\bType\\b"<< "\\bUnlock\\b"<< "\\bUntil\\b"<< "\\bUpdateGame\\b"<< "\\bUpKey\\b"<< "\\bUpper\\b"<< "\\bVerticalText\\b"<< "\\bWait\\b"<< "\\bWaitKey\\b"<< "\\bWaitMouse\\b"<< "\\bWend\\b"<< "\\bWhile\\b"<< "\\bWrapAngle\\b"<< "\\bWrite\\b"<< "\\bWriteByte\\b"<< "\\bWriteFloat\\b"<< "\\bWriteInt\\b"<< "\\bWriteLine\\b"<< "\\bWriteShort\\b"<< "\\bWriteString\\b"<< "\\bXor\\b";
    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern,Qt::CaseInsensitive);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    numberFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("\\b[0-9.]+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);


    //multiLineCommentFormat.setForeground(Qt::gray);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegExp("(//|')[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    /*functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);*/

    //commentStartExpression = QRegExp("/\\*");
    //commentEndExpression = QRegExp("\\*/");

}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
    /*
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);


    while (startIndex >= 0) {

        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
    */
}
