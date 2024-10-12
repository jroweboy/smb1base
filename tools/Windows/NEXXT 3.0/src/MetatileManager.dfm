object MetatileEditor: TMetatileEditor
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Metatile Editor'
  ClientHeight = 432
  ClientWidth = 487
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Image2: TImage
    Left = 2
    Top = 148
    Width = 256
    Height = 256
  end
  object SpeedButton6: TSpeedButton
    Left = 135
    Top = 57
    Width = 35
    Height = 19
    Caption = 'Clear'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton4: TSpeedButton
    Left = 2
    Top = 410
    Width = 76
    Height = 19
    Caption = 'Add from map'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton8: TSpeedButton
    Left = 159
    Top = 410
    Width = 41
    Height = 19
    Caption = 'unused'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton9: TSpeedButton
    Left = 84
    Top = 410
    Width = 37
    Height = 19
    Caption = 'Sort'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton10: TSpeedButton
    Left = 206
    Top = 410
    Width = 52
    Height = 19
    Caption = 'duplicates'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 127
    Top = 414
    Width = 29
    Height = 13
    Caption = 'Clear:'
  end
  object GroupBox1: TGroupBox
    Left = 2
    Top = 2
    Width = 173
    Height = 145
    Caption = 'Metatile'
    TabOrder = 0
    object Image1: TImage
      Left = 3
      Top = 14
      Width = 128
      Height = 128
    end
    object Label2: TLabel
      Left = 137
      Top = 12
      Width = 24
      Height = 12
      Caption = 'Freq.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SpeedButton2: TSpeedButton
      Left = 135
      Top = 103
      Width = 35
      Height = 16
      Caption = 'Copy'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SpeedButton3: TSpeedButton
      Left = 135
      Top = 123
      Width = 35
      Height = 16
      Caption = 'Paste'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SpeedButton5: TSpeedButton
      Left = 135
      Top = 84
      Width = 35
      Height = 16
      Caption = 'Cut'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SpeedButton7: TSpeedButton
      Left = 135
      Top = 64
      Width = 35
      Height = 16
      Caption = 'Clear'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label1: TLabel
      Left = 148
      Top = 38
      Width = 22
      Height = 12
      Caption = 'deep'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 148
      Top = 49
      Width = 16
      Height = 12
      Caption = 'edit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 137
      Top = 24
      Width = 5
      Height = 12
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object CheckBox2: TCheckBox
      Left = 133
      Top = 40
      Width = 14
      Height = 17
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 178
    Top = 2
    Width = 80
    Height = 145
    Caption = 'List item (set)'
    TabOrder = 1
    object Insert1: TSpeedButton
      Left = 4
      Top = 15
      Width = 72
      Height = 19
      Caption = 'Insert'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Remove1: TSpeedButton
      Left = 4
      Top = 36
      Width = 42
      Height = 19
      Caption = 'Remove'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Duplicate1: TSpeedButton
      Tag = 1
      Left = 4
      Top = 57
      Width = 72
      Height = 19
      Caption = 'Duplicate'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Up1: TSpeedButton
      Left = 4
      Top = 78
      Width = 35
      Height = 19
      Caption = 'Up'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Down1: TSpeedButton
      Left = 41
      Top = 78
      Width = 35
      Height = 19
      Caption = 'Down'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Clear1: TSpeedButton
      Left = 48
      Top = 36
      Width = 28
      Height = 19
      Caption = 'Clear'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Copy1: TSpeedButton
      Left = 4
      Top = 120
      Width = 35
      Height = 19
      Caption = 'Copy'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Paste1: TSpeedButton
      Left = 41
      Top = 120
      Width = 35
      Height = 19
      Caption = 'Paste'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SpeedButton1: TSpeedButton
      Left = 3
      Top = 99
      Width = 72
      Height = 19
      Caption = 'Rename'
    end
  end
  object GroupBox3: TGroupBox
    Left = 264
    Top = 2
    Width = 221
    Height = 402
    Caption = 'List (by metatile type)'
    TabOrder = 2
    object PageControl1: TPageControl
      Left = 4
      Top = 15
      Width = 211
      Height = 382
      ActivePage = TabSheet4x4
      TabOrder = 0
      object TabSheet2x2: TTabSheet
        Caption = '2x2'
        ExplicitWidth = 192
        object ListBox1: TListBox
          Left = 0
          Top = 0
          Width = 202
          Height = 353
          ExtendedSelect = False
          ItemHeight = 13
          TabOrder = 0
        end
      end
      object TabSheet4x4: TTabSheet
        Caption = '4x4'
        ExplicitWidth = 192
        ExplicitHeight = 172
        object ListBox2: TListBox
          Left = 0
          Top = 0
          Width = 202
          Height = 353
          ExtendedSelect = False
          ItemHeight = 13
          TabOrder = 0
        end
      end
    end
  end
  object CheckBox1: TCheckBox
    Left = 264
    Top = 413
    Width = 215
    Height = 17
    Caption = '<- within active list item / set'
    TabOrder = 3
  end
end
