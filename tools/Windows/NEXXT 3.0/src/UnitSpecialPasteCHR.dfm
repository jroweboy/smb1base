object FormSpecialPasteCHR: TFormSpecialPasteCHR
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'CHR Special Paste toolbox'
  ClientHeight = 227
  ClientWidth = 309
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 2
    Top = 187
    Width = 306
    Height = 38
    Caption = 'Paste as (applies to mode A, on top/behind, OR/AND/NAND)'
    TabOrder = 0
    object RadioAsPattern: TRadioButton
      Left = 6
      Top = 16
      Width = 60
      Height = 17
      Caption = 'Normal'
      Checked = True
      TabOrder = 0
      OnMouseEnter = RadioAsPatternMouseEnter
    end
    object RadioAsSilhouette: TRadioButton
      Left = 64
      Top = 16
      Width = 144
      Height = 17
      Caption = 'Silhouette (active colour)'
      TabOrder = 1
      OnMouseEnter = RadioAsSilhouetteMouseEnter
    end
    object RadioAsInvSilhouette: TRadioButton
      Left = 208
      Top = 16
      Width = 92
      Height = 17
      Caption = 'Inv. silhouette '
      TabOrder = 2
      OnMouseEnter = RadioAsInvSilhouetteMouseEnter
    end
  end
  object GroupBox2: TGroupBox
    Left = 2
    Top = 1
    Width = 306
    Height = 40
    Caption = 'Mode A: colour masked paste (shift+ctrl+v)'
    TabOrder = 1
    object btn0: TSpeedButton
      Left = 199
      Top = 14
      Width = 23
      Height = 19
      AllowAllUp = True
      GroupIndex = 1
      Caption = '0'
    end
    object btn1: TSpeedButton
      Left = 223
      Top = 14
      Width = 23
      Height = 19
      AllowAllUp = True
      GroupIndex = 2
      Caption = '1'
    end
    object btn2: TSpeedButton
      Left = 247
      Top = 14
      Width = 23
      Height = 19
      AllowAllUp = True
      GroupIndex = 3
      Caption = '2'
    end
    object btn3: TSpeedButton
      Left = 271
      Top = 14
      Width = 23
      Height = 19
      AllowAllUp = True
      GroupIndex = 4
      Down = True
      Caption = '3'
    end
    object RadioSolids: TRadioButton
      Left = 6
      Top = 16
      Width = 48
      Height = 17
      Caption = 'Solids'
      Checked = True
      TabOrder = 0
      OnMouseEnter = RadioSolidsMouseEnter
    end
    object RadioCol0: TRadioButton
      Left = 70
      Top = 16
      Width = 48
      Height = 17
      Caption = 'Col 0'
      TabOrder = 1
      OnMouseEnter = RadioCol0MouseEnter
    end
    object RadioCustom: TRadioButton
      Left = 130
      Top = 16
      Width = 58
      Height = 17
      Caption = 'Custom:'
      TabOrder = 2
      OnMouseEnter = RadioCustomMouseEnter
    end
  end
  object GroupBox3: TGroupBox
    Left = 2
    Top = 41
    Width = 306
    Height = 73
    Caption = 'Mode B: (ctrl+alt+v)'
    TabOrder = 2
    object CheckUseMaskB: TCheckBox
      Left = 6
      Top = 52
      Width = 164
      Height = 17
      TabStop = False
      Caption = 'Use Mode A colour mask'
      TabOrder = 0
      OnMouseEnter = CheckUseMaskBMouseEnter
    end
    object RadioAdd: TRadioButton
      Left = 6
      Top = 16
      Width = 40
      Height = 17
      Caption = 'Add'
      TabOrder = 1
      OnMouseEnter = RadioAddMouseEnter
    end
    object RadioKeepSim: TRadioButton
      Left = 172
      Top = 34
      Width = 118
      Height = 17
      Caption = 'Keep colour matches'
      TabOrder = 2
      OnMouseEnter = RadioKeepSimMouseEnter
    end
    object RadioKeepMask: TRadioButton
      Left = 172
      Top = 52
      Width = 124
      Height = 17
      Caption = 'Keep pixels in mask'
      Checked = True
      TabOrder = 3
      OnMouseEnter = RadioKeepMaskMouseEnter
    end
    object RadioBitOR: TRadioButton
      Left = 6
      Top = 34
      Width = 76
      Height = 16
      Caption = 'Bitwise OR'
      TabOrder = 4
      OnMouseEnter = RadioBitORMouseEnter
    end
    object RadioBitAND: TRadioButton
      Left = 88
      Top = 34
      Width = 78
      Height = 17
      Caption = 'Bitwise AND'
      TabOrder = 5
      OnMouseEnter = RadioBitANDMouseEnter
    end
    object RadioAdd1: TRadioButton
      Left = 52
      Top = 16
      Width = 54
      Height = 17
      Caption = 'Add -1'
      TabOrder = 6
      OnMouseEnter = RadioAdd1MouseEnter
    end
    object RadioAdd2: TRadioButton
      Left = 112
      Top = 16
      Width = 54
      Height = 17
      Caption = 'Add -2'
      TabOrder = 7
      OnMouseEnter = RadioAdd2MouseEnter
    end
    object RadioAdd3: TRadioButton
      Left = 172
      Top = 16
      Width = 54
      Height = 17
      Caption = 'Add -3'
      TabOrder = 8
      OnMouseEnter = RadioAdd3MouseEnter
    end
    object RadioOnTop: TRadioButton
      Left = 246
      Top = 16
      Width = 52
      Height = 17
      Caption = 'On top'
      TabOrder = 9
      OnMouseEnter = RadioOnTopMouseEnter
    end
  end
  object GroupBox4: TGroupBox
    Left = 2
    Top = 114
    Width = 306
    Height = 73
    Caption = 'Mode C: (shift+ctrl+alt+v)'
    TabOrder = 3
    object CheckUseMaskC: TCheckBox
      Left = 6
      Top = 52
      Width = 154
      Height = 17
      TabStop = False
      Caption = 'Use Mode A colour mask'
      TabOrder = 0
      OnMouseEnter = CheckUseMaskCMouseEnter
    end
    object RadioSub: TRadioButton
      Left = 6
      Top = 16
      Width = 40
      Height = 17
      Caption = 'Sub'
      TabOrder = 1
      OnMouseEnter = RadioSubMouseEnter
    end
    object RadioKeepDiff: TRadioButton
      Left = 172
      Top = 34
      Width = 131
      Height = 17
      Caption = 'Keep different colours'
      TabOrder = 2
      OnMouseEnter = RadioKeepDiffMouseEnter
    end
    object RadioRemoveMask: TRadioButton
      Left = 172
      Top = 52
      Width = 128
      Height = 17
      Caption = 'Remove pixels in mask'
      Checked = True
      TabOrder = 3
      OnMouseEnter = RadioRemoveMaskMouseEnter
    end
    object RadioBitNAND: TRadioButton
      Left = 82
      Top = 34
      Width = 84
      Height = 17
      Caption = 'Bitwise NAND'
      TabOrder = 4
      OnClick = RadioBitNANDClick
    end
    object RadioSub1: TRadioButton
      Left = 52
      Top = 16
      Width = 56
      Height = 17
      Caption = 'Sub +1'
      TabOrder = 5
      OnMouseEnter = RadioSub1MouseEnter
    end
    object RadioSub2: TRadioButton
      Left = 112
      Top = 16
      Width = 56
      Height = 17
      Caption = 'Sub +2'
      TabOrder = 6
      OnMouseEnter = RadioSub2MouseEnter
    end
    object RadioSub3: TRadioButton
      Left = 172
      Top = 16
      Width = 56
      Height = 17
      Caption = 'Sub +3'
      TabOrder = 7
      OnClick = RadioSub3Click
    end
    object RadioBehind: TRadioButton
      Left = 246
      Top = 16
      Width = 52
      Height = 17
      Caption = 'Behind'
      TabOrder = 8
    end
    object CheckSubmask: TCheckBox
      Left = 6
      Top = 34
      Width = 66
      Height = 17
      Caption = 'Sub mask'
      Checked = True
      State = cbChecked
      TabOrder = 9
      OnMouseEnter = CheckSubmaskMouseEnter
    end
  end
end
