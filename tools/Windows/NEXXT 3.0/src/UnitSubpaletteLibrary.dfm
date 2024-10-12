object SubpaletteLibrary1: TSubpaletteLibrary1
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Subpalette library'
  ClientHeight = 507
  ClientWidth = 281
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ScrollBox1: TScrollBox
    Left = 3
    Top = 8
    Width = 277
    Height = 277
    TabOrder = 0
    object PaintBox1: TPaintBox
      Left = 0
      Top = 0
      Width = 256
      Height = 256
    end
  end
  object GroupBox1: TGroupBox
    Left = 3
    Top = 402
    Width = 142
    Height = 105
    Caption = 'File'
    TabOrder = 1
    object SpeedButton1: TSpeedButton
      Left = 3
      Top = 16
      Width = 62
      Height = 22
      Caption = 'Load'
    end
    object SpeedButton2: TSpeedButton
      Left = 71
      Top = 16
      Width = 62
      Height = 22
      Caption = 'Add'
    end
    object RadioButton1: TRadioButton
      Left = 3
      Top = 44
      Width = 81
      Height = 17
      Caption = 'From folder'
      TabOrder = 0
    end
    object RadioButton2: TRadioButton
      Left = 3
      Top = 60
      Width = 73
      Height = 17
      Caption = 'From File'
      TabOrder = 1
    end
  end
end
