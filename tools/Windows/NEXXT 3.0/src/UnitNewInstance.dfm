object FormNewInstance: TFormNewInstance
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'New instance'
  ClientHeight = 256
  ClientWidth = 452
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 2
    Top = 0
    Width = 447
    Height = 222
    Caption = 'Inherit from this session...'
    TabOrder = 0
    object GroupBox2: TGroupBox
      Left = 3
      Top = 15
      Width = 238
      Height = 139
      Caption = 'Tileset'
      TabOrder = 0
      object RadioButton1: TRadioButton
        Left = 8
        Top = 16
        Width = 113
        Height = 17
        Caption = 'None'
        TabOrder = 0
      end
      object RadioButton2: TRadioButton
        Left = 8
        Top = 48
        Width = 113
        Height = 17
        Caption = 'Current set'
        TabOrder = 1
      end
      object RadioButton3: TRadioButton
        Left = 8
        Top = 64
        Width = 113
        Height = 17
        Caption = 'Current 4 sets'
        TabOrder = 2
      end
      object RadioButton4: TRadioButton
        Left = 8
        Top = 81
        Width = 153
        Height = 17
        Caption = 'All banks, including mapping'
        TabOrder = 3
      end
      object RadioButton5: TRadioButton
        Left = 8
        Top = 32
        Width = 113
        Height = 17
        Caption = 'Selection'
        TabOrder = 4
      end
      object RadioButton6: TRadioButton
        Left = 8
        Top = 98
        Width = 220
        Height = 17
        Caption = 'None, but link this session to new instance'
        TabOrder = 5
      end
      object CheckBox5: TCheckBox
        Left = 8
        Top = 116
        Width = 148
        Height = 17
        Caption = 'Incl. chr collision properties'
        TabOrder = 6
      end
    end
    object GroupBox3: TGroupBox
      Left = 243
      Top = 15
      Width = 200
      Height = 139
      Caption = 'Nametable/Map'
      TabOrder = 1
      object RadioButton7: TRadioButton
        Left = 8
        Top = 16
        Width = 113
        Height = 17
        Caption = 'None'
        TabOrder = 0
      end
      object RadioButton8: TRadioButton
        Left = 8
        Top = 48
        Width = 177
        Height = 17
        Caption = 'Selection + canvas dimensions'
        TabOrder = 1
      end
      object RadioButton9: TRadioButton
        Left = 8
        Top = 64
        Width = 177
        Height = 17
        Caption = 'Canvas contents + dimensions'
        TabOrder = 2
      end
      object RadioButton10: TRadioButton
        Left = 8
        Top = 81
        Width = 153
        Height = 17
        Caption = 'Just selection dimensions'
        TabOrder = 3
      end
      object RadioButton11: TRadioButton
        Left = 8
        Top = 32
        Width = 177
        Height = 17
        Caption = 'Selection + selection dimensions'
        TabOrder = 4
      end
      object RadioButton12: TRadioButton
        Left = 8
        Top = 98
        Width = 153
        Height = 17
        Caption = 'Just canvas dimensions'
        TabOrder = 5
      end
    end
    object GroupBox4: TGroupBox
      Left = 3
      Top = 153
      Width = 440
      Height = 64
      Caption = 'Misc.'
      TabOrder = 2
      object CheckBox1: TCheckBox
        Left = 8
        Top = 16
        Width = 97
        Height = 17
        Caption = 'Subpalettes'
        TabOrder = 0
      end
      object CheckBox3: TCheckBox
        Left = 100
        Top = 16
        Width = 80
        Height = 17
        Caption = 'Metasprites'
        TabOrder = 1
      end
      object CheckBox2: TCheckBox
        Left = 208
        Top = 16
        Width = 125
        Height = 17
        Caption = 'Bitwise collision labels'
        TabOrder = 2
      end
      object CheckBox4: TCheckBox
        Left = 348
        Top = 16
        Width = 90
        Height = 17
        Caption = 'Collision ID list'
        TabOrder = 3
      end
      object CheckBox6: TCheckBox
        Left = 8
        Top = 36
        Width = 68
        Height = 17
        Caption = 'Metatiles'
        TabOrder = 4
      end
      object CheckBox7: TCheckBox
        Left = 100
        Top = 36
        Width = 80
        Height = 17
        Caption = 'Brush Masks'
        TabOrder = 5
      end
      object CheckBox8: TCheckBox
        Left = 348
        Top = 36
        Width = 80
        Height = 17
        Caption = 'Button state'
        TabOrder = 6
      end
    end
  end
  object Button1: TButton
    Left = 374
    Top = 228
    Width = 75
    Height = 25
    Caption = 'Make && Close'
    TabOrder = 1
  end
  object Button2: TButton
    Left = 218
    Top = 228
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 2
  end
  object Button3: TButton
    Left = 296
    Top = 228
    Width = 75
    Height = 25
    Caption = 'Make'
    TabOrder = 3
  end
  object Button4: TButton
    Left = 2
    Top = 228
    Width = 47
    Height = 25
    Caption = 'All'
    TabOrder = 4
  end
  object Button5: TButton
    Left = 50
    Top = 228
    Width = 47
    Height = 25
    Caption = 'None'
    TabOrder = 5
  end
  object Button6: TButton
    Left = 98
    Top = 228
    Width = 47
    Height = 25
    Caption = 'Preset A'
    TabOrder = 6
  end
  object Button7: TButton
    Left = 146
    Top = 228
    Width = 47
    Height = 25
    Caption = 'Preset B'
    TabOrder = 7
  end
end
